function curr = itertrap(question3f, a, b, nmax, tol)
    i = 0;
    s = 0;
    h = (b-a)/2;
    fprintf('Starting integral approx\n');
    while (i < nmax)
       if ( i == 0)
           curr = h * (question3f(a) + question3f(b));
       else
         k = 1;
         h = (b-a) / 2^i;
         while (k <= 2^(i-1))
            s = s + question3f(a + ((2*k - 1)*h));
            k=k+1;
         end
         curr = (.5 * last) + (s * h);
         fprintf('%i\t', i), fprintf('%10.10f\n', curr);
         if (abs(1 - last/curr) < tol)
           return
         end
       end
       i = i + 1;
       s = 0;
       last = curr;
    end
    fprintf('Failed to converge in %i', nmax), fprintf('iterations\n')