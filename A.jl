using JuMP, CPLEX, Test

model = direct_model(CPLEX.Optimizer())
set_silent(model)

# This is very, very important!!! Only use callbacks in single-threaded mode.
MOI.set(model, MOI.NumberOfThreads(), 1)

@variable(model, 0 <= x <= 2.5, Int)
@variable(model, 0 <= y <= 2.5, Int)
@objective(model, Max, y)
cb_calls = Clong[]
function my_callback_function(cb_data::CPLEX.CallbackContext, context_id::Clong)
    # You can reference variables outside the function as normal
    push!(cb_calls, context_id)
    # You can select where the callback is run
    if context_id != CPX_CALLBACKCONTEXT_CANDIDATE
        return
    end
    ispoint_p = Ref{Cint}()
    ret = CPXcallbackcandidateispoint(cb_data, ispoint_p)
    if ret != 0 || ispoint_p[] == 0
        return  # No candidate point available or error
    end
    # You can query CALLBACKINFO items
    valueP = Ref{Cdouble}()
    ret = CPXcallbackgetinfodbl(cb_data, CPXCALLBACKINFO_BEST_BND, valueP)
    @info "Best bound is currently: $(valueP[])"
    # As well as any other C API
    x_p = Vector{Cdouble}(undef, 2)
    obj_p = Ref{Cdouble}()
    ret = CPXcallbackgetincumbent(cb_data, x_p, 0, 1, obj_p)
    if ret == 0
        @info "Objective incumbent is: $(obj_p[])"
        @info "Incumbent solution is: $(x_p)"
        # Use CPLEX.column to map between variable references and the 1-based
        # column.
        x_col = CPLEX.column(cb_data, index(x))
        @info "x = $(x_p[x_col])"
    else
        # Unable to query incumbent.
    end

    # Before querying `callback_value`, you must call:
    CPLEX.load_callback_variable_primal(cb_data, context_id)
    x_val = callback_value(cb_data, x)
    y_val = callback_value(cb_data, y)
    # You can submit solver-independent MathOptInterface attributes such as
    # lazy constraints, user-cuts, and heuristic solutions.
    if y_val - x_val > 1 + 1e-6
        con = @build_constraint(y - x <= 1)
        MOI.submit(model, MOI.LazyConstraint(cb_data), con)
    elseif y_val + x_val > 3 + 1e-6
        con = @build_constraint(y + x <= 3)
        MOI.submit(model, MOI.LazyConstraint(cb_data), con)
    end
end
MOI.set(model, CPLEX.CallbackFunction(), my_callback_function)
optimize!(model)
@test termination_status(model) == MOI.OPTIMAL
@test primal_status(model) == MOI.FEASIBLE_POINT
@test value(x) == 1
@test value(y) == 2
