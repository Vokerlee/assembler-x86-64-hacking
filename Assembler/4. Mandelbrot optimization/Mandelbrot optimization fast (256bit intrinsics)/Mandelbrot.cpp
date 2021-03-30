#include "Mandelbrot.h"

// FPS = 39-40

void fill_data(sf::Uint32* data, int x_size, int y_size, double division, int shift_hor, int shift_ver)
{
	__m256i mask32_256_1 = _mm256_setr_epi32(0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	__m256i mask32_256_2 = _mm256_setr_epi32(0xFFFFFFFF, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	__m256i mask32_256_3 = _mm256_setr_epi32(0xFFFFFFFF, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	__m256i mask32_256_4 = _mm256_setr_epi32(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	__m256i mask32_256_5 = _mm256_setr_epi32(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	__m256i mask32_256_6 = _mm256_setr_epi32(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0xFFFFFFFF);
	__m256i mask32_256_7 = _mm256_setr_epi32(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0xFFFFFFFF);
	__m256i mask32_256_8 = _mm256_setr_epi32(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0);

	int x_left = -x_size / 2 + 1 + shift_hor;
	int x_right = x_size / 2 + x_size % 2 + shift_hor;
	int y_left = -y_size / 2 + 1 + shift_ver;
	int y_right = y_size / 2 + y_size % 2 + shift_ver;
	
	__m256i ones    = _mm256_set1_epi32(1);
	__m256i zeros   = _mm256_set1_epi32(0);
	__m256 sqradius = _mm256_set1_ps(SQ_RADIUS);

	for (int y = y_left; y <= y_right; ++y)
	{
		float y0 = y / division;
		__m256 im = _mm256_set1_ps(y0);

		for (int x = x_left; x <= x_right; x += 8)
		{
			__m256i iterations = _mm256_set1_epi32(0);
			__m256i iter_mask  = _mm256_set1_epi32(0xFFFFFFFF);

			float x0 = x / division;
			float dx = 1 / division;
			__m256 re = { x0, x0 + dx, x0 + 2 * dx, x0 + 3 * dx, x0 + 4 * dx, x0 + 5 * dx, x0 + 6 * dx, x0 + 7 * dx };

			__m256 iter_re = re;
			__m256 iter_im = im;

			for (int iters = 1; iters < ITERATION_LIMIT; ++iters)
			{
				__m256 sqx_re = _mm256_mul_ps(iter_re, iter_re);
				__m256 sqx_im = _mm256_mul_ps(iter_im, iter_im);

				__m256 current_radius = _mm256_add_ps(sqx_re, sqx_im);

				__m256i radius_compare = _mm256_add_epi32(_mm256_cvtps_epi32(_mm256_cmp_ps(current_radius, sqradius, _CMP_GT_OS)), ones); // INT_MIN + 1 is out, 1 is in

				radius_compare = _mm256_abs_epi32(_mm256_cmpgt_epi32(radius_compare, zeros));

				iterations = _mm256_add_epi32(iterations, _mm256_and_si256(iter_mask, radius_compare));

				if (*((int32_t*)&radius_compare) == 0)
					iter_mask = _mm256_and_si256(iter_mask, mask32_256_1);

				if (*((int32_t*)&radius_compare + 1) == 0)
					iter_mask = _mm256_and_si256(iter_mask, mask32_256_2);

				if (*((int32_t*)&radius_compare + 2) == 0)
					iter_mask = _mm256_and_si256(iter_mask, mask32_256_3);

				if (*((int32_t*)&radius_compare + 3) == 0)
					iter_mask = _mm256_and_si256(iter_mask, mask32_256_4);

				if (*((int32_t*)&radius_compare + 4) == 0)
					iter_mask = _mm256_and_si256(iter_mask, mask32_256_5);

				if (*((int32_t*)&radius_compare + 5) == 0)
					iter_mask = _mm256_and_si256(iter_mask, mask32_256_6);

				if (*((int32_t*)&radius_compare + 6) == 0)
					iter_mask = _mm256_and_si256(iter_mask, mask32_256_7);

				if (*((int32_t*)&radius_compare + 7) == 0)
					iter_mask = _mm256_and_si256(iter_mask, mask32_256_8);

				if (_mm256_test_all_zeros(iter_mask, iter_mask))
					break;

				iter_im = _mm256_add_ps(iter_im, iter_im);
				iter_im = _mm256_mul_ps(iter_im, iter_re);
				iter_im = _mm256_add_ps(iter_im, im);

				iter_re = _mm256_sub_ps(sqx_re, sqx_im);
				iter_re = _mm256_add_ps(iter_re, re);
			}

			colour clr = {};
			clr.number = clr.number | 0xcc000000;

			clr.colour = RGBQUAD{ (BYTE)(180 - *((int32_t*)&iterations) - (*((int32_t*)&iterations) % 2) * 120), (BYTE)(10 + 25 * (*((int32_t*)&iterations) % 2)), (BYTE)(*((int32_t*)&iterations) * 11) };
			clr.number = clr.number | 0xcc000000;
			data[(y - y_left) * x_size + x - x_left] = clr.number;

			clr.colour = RGBQUAD{ (BYTE)(180 - *((int32_t*)&iterations + 1) - (*((int32_t*)&iterations + 1) % 2) * 120), (BYTE)(10 + 25 * (*((int32_t*)&iterations + 1) % 2)), (BYTE)(*((int32_t*)&iterations + 1) * 11) };
			clr.number = clr.number | 0xcc000000;
			data[(y - y_left) * x_size + x + 1 - x_left] = clr.number;

			clr.colour = RGBQUAD{ (BYTE)(180 - *((int32_t*)&iterations + 2) - (*((int32_t*)&iterations + 2) % 2) * 120), (BYTE)(10 + 25 * (*((int32_t*)&iterations + 2) % 2)), (BYTE)(*((int32_t*)&iterations + 2) * 11) };
			clr.number = clr.number | 0xcc000000;
			data[(y - y_left) * x_size + x + 2 - x_left] = clr.number;

			clr.colour = RGBQUAD{ (BYTE)(180 - *((int32_t*)&iterations + 3) - (*((int32_t*)&iterations + 3) % 2) * 120), (BYTE)(10 + 25 * (*((int32_t*)&iterations + 3) % 2)), (BYTE)(*((int32_t*)&iterations + 3) * 11) };
			clr.number = clr.number | 0xcc000000;
			data[(y - y_left) * x_size + x + 3 - x_left] = clr.number;

			clr.colour = RGBQUAD{ (BYTE)(180 - *((int32_t*)&iterations + 4) - (*((int32_t*)&iterations + 4) % 2) * 120), (BYTE)(10 + 25 * (*((int32_t*)&iterations + 4) % 2)), (BYTE)(*((int32_t*)&iterations + 4) * 11) };
			clr.number = clr.number | 0xcc000000;
			data[(y - y_left) * x_size + x + 4 - x_left] = clr.number;

			clr.colour = RGBQUAD{ (BYTE)(180 - *((int32_t*)&iterations + 5) - (*((int32_t*)&iterations + 5) % 2) * 120), (BYTE)(10 + 25 * (*((int32_t*)&iterations + 5) % 2)), (BYTE)(*((int32_t*)&iterations + 5) * 11) };
			clr.number = clr.number | 0xcc000000;
			data[(y - y_left) * x_size + x + 5 - x_left] = clr.number;

			clr.colour = RGBQUAD{ (BYTE)(180 - *((int32_t*)&iterations + 6) - (*((int32_t*)&iterations + 6) % 2) * 120), (BYTE)(10 + 25 * (*((int32_t*)&iterations + 6) % 2)), (BYTE)(*((int32_t*)&iterations + 6) * 11) };
			clr.number = clr.number | 0xcc000000;
			data[(y - y_left) * x_size + x + 6 - x_left] = clr.number;

			clr.colour = RGBQUAD{ (BYTE)(180 - *((int32_t*)&iterations + 7) - (*((int32_t*)&iterations + 7) % 2) * 120), (BYTE)(10 + 25 * (*((int32_t*)&iterations + 7) % 2)), (BYTE)(*((int32_t*)&iterations + 7) * 11) };
			clr.number = clr.number | 0xcc000000;
			data[(y - y_left) * x_size + x + 7 - x_left] = clr.number;
		}
	}
}

void print_figure(int x_size, int y_size)
{
	double division = INITIAL_DIVISION;
	int shift_hor = 0;
	int shift_ver = 0;

	sf::Uint32* data = new sf::Uint32[Y_SIZE * X_SIZE];

	sf::RenderWindow window(sf::VideoMode(x_size, y_size), "Mandelbrot");

	sf::Texture mandelbrot;

	mandelbrot.create(x_size, y_size);
	mandelbrot.update((sf::Uint8*)data, x_size, y_size, 0, 0);

	sf::Sprite table(mandelbrot);
	table.setPosition(0, 0);

	size_t  fps_counter = 0;
	clock_t time = clock();

	while (window.isOpen())
	{
		float fps = (fps_counter++) * CLOCKS_PER_SEC / ((float)clock() - time);
		printf("%f" "\n", fps);

		fill_data(data, X_SIZE, Y_SIZE, division, shift_hor, shift_ver);
		mandelbrot.update((sf::Uint8*)data, x_size, y_size, 0, 0);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseWheelMoved)
				division += 5 * event.mouseWheel.delta;

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Left)
					shift_hor -= 20;

				if (event.key.code == sf::Keyboard::Right)
					shift_hor += 20;

				if (event.key.code == sf::Keyboard::Up)
					shift_ver += 20;

				if (event.key.code == sf::Keyboard::Down)
					shift_ver -= 20;
			}
		}

		window.clear();
		window.draw(table);
		window.display();
	}

	delete[] data;
}