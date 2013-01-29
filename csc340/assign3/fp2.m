function [ out ] = fp2( x )
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here
    out = polyval(polyder(polyder([1 -4.6 -1.12 22.08 -0.8 -30.56 -14.08])), x)
end

