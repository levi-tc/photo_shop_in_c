#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>
#define LIMITA_255 255

void swap_m(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

// TODO Task 1
int ***flip_horizontal(int ***image, int N, int M)
{

	for (int j = 0; j < M / 2; j++)
	{

		for (int i = 0; i < N; i++)
		{
			for (int k = 0; k < 3; k++)
			{
				swap_m(&image[i][j][k], &image[i][M - j - 1][k]);
			}
		}
	}

	return image;
}

// TODO Task 2
int ***rotate_left(int ***image, int N, int M)
{

	int ***m_rotated = (int ***)malloc(M * sizeof(int **));
	for (int i = 0; i < M; i++)
	{
		m_rotated[i] = (int **)malloc(N * sizeof(int *));
		for (int j = 0; j < N; j++)
		{
			m_rotated[i][j] = (int *)malloc(3 * sizeof(int));
		}
	}

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				m_rotated[i][j][k] = image[j][M - i - 1][k];
			}
		}
	}

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			free(image[i][j]);
		}
		free(image[i]);
	}
	free(image);
	return m_rotated;
}

// TODO Task 3
int ***crop(int ***image, int N, int M, int x, int y, int h, int w)
{
	int ***m_cropped = (int ***)malloc(h * sizeof(int **));
	for (int i = 0; i < h; i++)
	{
		m_cropped[i] = (int **)malloc(w * sizeof(int *));
		for (int j = 0; j < w; j++)
		{
			m_cropped[i][j] = (int *)malloc(3 * sizeof(int));
		}
	}

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				m_cropped[i][j][k] = image[y + i][x + j][k];
			}
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			free(image[i][j]);
		}
		free(image[i]);
	}
	free(image);
	return m_cropped;
}

// TODO Task 4
int ***extend(int ***image, int N, int M, int rows, int cols, int new_R,
              int new_G, int new_B)
{
    int ***m_extended = (int ***)malloc((N + 2 * rows) * sizeof(int **));
    for (int i = 0; i < N + 2 * rows; i++)
    {
        m_extended[i] = (int **)malloc((M + 2 * cols) * sizeof(int *));
        for (int j = 0; j < M + 2 * cols; j++)
        {
            m_extended[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }

    for (int i = 0; i < N + 2 * rows; i++)
    {
        for (int j = 0; j < M + 2 * cols; j++)
        {
            m_extended[i][j][0] = new_R;
            m_extended[i][j][1] = new_G;
            m_extended[i][j][2] = new_B;
        }
    }

    for (int i = rows; i < N + rows; i++)
    {
        for (int j = cols; j < M + cols; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                m_extended[i][j][k] = image[i - rows][j - cols][k];
            }
        }
    }

    // Free the memory of the original image
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);

    return m_extended;
}


// TODO Task 5
int ***paste(int ***image_dst, int N_dst, int M_dst, int ***image_src,
			 int N_src, int M_src, int x, int y)
{
	int ***m_copy = image_dst;
	for (int i = 0; i < N_dst; i++)
	{
		for (int j = 0; j < M_dst; j++)

		{
			for (int k = 0; k < 3; k++)
			{
				m_copy[i][j][k] = image_dst[i][j][k];
			}
		}
	}
	for (int i = 0; i < N_src; i++)
	{
		for (int j = 0; j < M_src; j++)
		{
			if (x + j < M_dst && y + i < N_dst)
				for (int k = 0; k < 3; k++)
				{
					m_copy[i + y][j + x][k] = image_src[i][j][k];
				}
		}
	}

	return m_copy;
}

// TODO Task 6
int clamp(int x, int min, int max)
{
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}

int ***apply_filter(int ***image, int N, int M, float **filter,
					int filter_size)
{

	int ***m_filter = (int ***)malloc(N * sizeof(int **));
	for (int i = 0; i < N; i++)
	{
		m_filter[i] = (int **)malloc(M * sizeof(int *));
		for (int j = 0; j < M; j++)
		{
			m_filter[i][j] = (int *)malloc(3 * sizeof(int));
		}
	}
		int filter_radius = filter_size / 2;

		for(int i=0;i<N;i++){
			for(int j=0;j<M;j++){
				float sum_R=0, sum_G=0, sum_B=0;

				for(int x= -filter_radius;x<=filter_radius;x++){
					for(int y= -filter_radius;y<=filter_radius;y++){
						int a = x+i;
						int b = y+j;
						if(a>= 0 && a<N && b>=0 && b<M){
							sum_R += image[a][b][0] * filter[x+filter_radius][y+filter_radius];
							sum_G += image[a][b][1] * filter[x+filter_radius][y+filter_radius];
							sum_B += image[a][b][2] * filter[x+filter_radius][y+filter_radius];
						}
						else{
							sum_R += 0;
							sum_G += 0;
							sum_B += 0;
						}
					}
				}
				m_filter[i][j][0] = clamp((int)sum_R,0,LIMITA_255);
				m_filter[i][j][1] = clamp((int)sum_G,0,LIMITA_255);
				m_filter[i][j][2] = clamp((int)sum_B,0,LIMITA_255);
			}

		}

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < M; j++)
			{

				free(image[i][j]);
			}
			free(image[i]);
		}
		free(image);

	return m_filter;
}
