function root=newton(f,fp,x0,nmax,eps)
%Newton.M Performs newtons method on a polynomial
%   Csc340 assignment 3.
%	Braden Simpson.
%	V00685500
i=1;
fprintf('iteration   approximation \n')
while i<=nmax
	root=x0-f(x0)/fp(x0); 
	fprintf('%6.0f',i),fprintf('%18.10f\n',root)
	if(abs(1-x0/root))<eps
		return
	end
	i=i+1;
	x0=root;
end
fprintf('failed to converge in %g',nmax),fprintf('  iterations\n')    
