mass = importdata("test.txt");

graph = figure;

x = 1:1:1009;
y = mass(:, 1);

disp = 1 / (length(y)-1) * sum((y - mean(y)).^2)

bar(x, y);

ylabel('Plenum');
xlabel('Cells');

title('CRC32 hash');
grid on

set(graph, 'visible', 'on');
saveas(graph, 'CRC32 hash.png');