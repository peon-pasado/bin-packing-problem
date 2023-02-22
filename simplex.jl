using JuMP
using CPLEX

function setup(s, n, B)
    model = Model(CPLEX.Optimizer)
    #add feasible constraints
    pi_constratins = get_pi(model)
    
    
    column = get_column(s, n, B, pi_constratins)

end



#=
model = Model(CPLEX.Optimizer)
@variable(model, x[1:2])
@constraint(model, c1, x[1] + x[2] <= 1)
@constraint(model, c2, x[1] - x[2] <= 1)
@constraint(model, c3,  -0.5 <= x[2] <= 0.5)
@objective(model, Max, x[1])

optimize!(model)

println(dual(c1))
println(dual(c2))
println(dual(c3))
=#