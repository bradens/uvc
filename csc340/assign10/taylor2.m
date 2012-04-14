function taylor2
	fprintf('values of t    approximation of x \n')
    t = 1;
    x = 2;
    h = 0.01;
    fprintf('%8.2f', t), fprintf('%19.8f\n',x)
    for i  = 1:20
        x = x + (0.01 * (1 + x / t)) + ((0.01^2)/2)*(-1-x+(t^3)+(t^2)*x);
        t = t + h;
        fprintf('%8.2f', t), fprintf('%19.8f\n', x)
    end