#include "Mandelbrot.h"

void fill_data(sf::Uint32* data, int x_size, int y_size) 
{
	int x_left = -x_size / 2 + 1;
	int x_right = x_size / 2 + x_size % 2;
	int y_left = -y_size / 2 + 1;
	int y_right = y_size / 2 + y_size % 2;

	double R2 = RADIUS * RADIUS;

	char state = IN_CIRCLE;

	for (int y = y_left; y <= y_right; ++y)
	{
		for (int x = x_left; x <= x_right; ++x)
		{
			state = IN_CIRCLE;

			int iterations = 0;

			double x0 = x / INITIAL_DIVISION;
			double y0 = y / INITIAL_DIVISION;

			double x1 = 0;
			double y1 = 0;

			for (iterations = 0; iterations < ITERATION_LIMIT; ++iterations)
			{
				double sqx = x0 * x0;
				double sqy = y0 * y0;

				if (sqx + sqy > R2)
				{
					state = OUT_CIRCLE;
					break;
				}

				x1 = sqx - sqy + x / INITIAL_DIVISION;
				y1 = 2 * x0 * y0 + y / INITIAL_DIVISION;
				//printf("for x = %d, y  = %d, rad = %lg, R2 = %lg\n", x, y, sqx + sqy, R2);

				x0 = x1;
				y0 = y1;
			}

			colour clr;
			clr.colour = RGBQUAD{ (BYTE) (180 - iterations - (iterations % 2) * 120), (BYTE) (10 + 25 * (iterations % 2)), (BYTE) (iterations * 11)  };
			clr.number = clr.number | 0xcc000000;

			//printf("%x\n", clr.number);
			
			if (state == OUT_CIRCLE)
				data[(y - y_left) * x_size + x - x_left] = clr.number;
		}
	}
}

void print_figure(int x_size, int y_size)
{
	sf::Uint32* data = new sf::Uint32[Y_SIZE * X_SIZE];
	fill_data(data, X_SIZE, Y_SIZE);

	sf::RenderWindow window(sf::VideoMode(x_size, y_size), "Mandelbrot");

	sf::Texture mandelbrot;

	mandelbrot.create(x_size, y_size);
	mandelbrot.update((sf::Uint8*) data, x_size, y_size, 0, 0);

	sf::Sprite table(mandelbrot);
	table.setPosition(0, 0);

	size_t  fps_counter  = 0;
	clock_t time = clock();

	while (window.isOpen())
	{
		float fps = (fps_counter++) * CLOCKS_PER_SEC / ((float) clock() - time);
		printf("%f" "\n", fps);

		fill_data(data, X_SIZE, Y_SIZE);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(table);
		window.display();
	}

	delete[] data;
}