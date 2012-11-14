clc;
close all;
clear all;
format short eng

v1 = [0.05 0.25 0.4 0.7 1];
v2 = -v1(end:-1:1);
vector = 12*[v1 v2 v1 v2 v1 v2 v1 v2 v1 v2 v1 v2 v1 v2 v1 v2];

dt = 0.1e-3;
time = 1:length(vector);
time = time .* dt;

plot(time,abs(vector))

temp = [time' vector'];
VEC = dataset(temp);
dlmwrite('D:\Dropbox\PROJET VUE\02-Conception\LTSpice\slow.txt', temp);

