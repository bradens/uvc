function [ out ] = fp( x )
%UNTITLED2 Summary of this function goes here
%   Detailed explanaton goes here
    out = polyval(polyder([1 -4.6 -1.12 22.08 -0.8 -30.56 -14.08]), x)
end