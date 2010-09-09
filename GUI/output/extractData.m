function extractData(fileName)
close all;
fin = fopen(fileName, 'r');

if fin < 0
   error(['Could not open ',fileName,' for input']);
end
%Read the Titel
m_titel = fgetl(fin);
fgetl(fin);
fgetl(fin);
fgetl(fin);
%Read timeintervall
buffer = fgetl(fin);
dT = strread(buffer, '%*[dT=]%d');
%read labels
buffer = fgetl(fin);
labels = strread(buffer, '%s');
nbr = length(labels);


if nbr == 1
    A = textread(fileName, '%n','headerlines',6);
end

if nbr == 2
    [A,B] = textread(fileName, '%n%n','headerlines',6);
end

if nbr == 3
    [A,B,C] = textread(fileName, '%n%n%n','headerlines',6);
end

time = 0:dT:dT*(length(A)-1);

if nbr == 1
    plot(time(1:length(A)),A)
end

if nbr == 2
     plotyy(time(1:length(A)),A,time,B)
     hold on;
end

legend(labels,0);
xlabel('Time [ms]');
ylabel('values');
title(m_titel);
hold off;

if nbr == 3
     figure(1)
  %   plot(time(1:length(A)),A,time,B)
    plot(time,A,time,B,time,C)
     hold on;
  %   plotyy(time,A,time,C);
   %  hold off;
     ylabel('values');
     xlabel('Time [ms]');
     grid on;
     title(m_titel);
     legend(labels,0);
     %legend(labels(2),0);
     % second Diagram
     figure(2)
     plot(time,C)
     title(m_titel);
     legend(labels(3),0);
end


xlabel('Time [ms]');
ylabel('values');
hold off;

maxValue = max(B)
minValue = min(B)
meanValue = mean(B)

fclose(fin);
