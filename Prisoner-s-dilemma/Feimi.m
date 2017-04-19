function result = Feimi(Centor, Neighbour)
K = 1;
x = 1/(1+exp((Centor - Neighbour)/K));
result = x - rand();
% C>N result<0
% C<N result >0