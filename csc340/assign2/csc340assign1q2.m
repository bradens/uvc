diary csc340assign1q2
a = sqrt(67.88^2 + 1) - 67.88
approx = sp_round(sp_round(sqrt(sp_round(sp_round(x^2, 4) + 1, 4)), 4) - 67.88, 4)
approx = sp_round(sp_round(sqrt(sp_round(sp_round((67.88^2), 4) + 1, 4)), 4) - 67.88, 4)
approx = sd_round(sd_round(sqrt(sd_round(sd_round((67.88^2), 4) + 1, 4)), 4) - 67.88, 4)
relErr = abs((a - approx))/abs(a)
