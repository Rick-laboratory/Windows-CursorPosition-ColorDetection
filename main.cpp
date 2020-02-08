#include<Windows.h>
#include <vector>
#include<iostream>
int main()
{
	POINT position;
	POINT positionPrev;
	positionPrev.x = 0;
	positionPrev.y = 0;

	int x = 0;
	int y = 0;
	int w = 1;
	int h = 1;
	BYTE _blu;
	BYTE _grn;
	BYTE _red;

	int screen_w = GetSystemMetrics(SM_CXFULLSCREEN);
	int screen_h = 1080;//GetSystemMetrics(SM_CYFULLSCREEN);

	while (true)
	{
		if (GetCursorPos(&position)) {

			//if (positionPrev.x != position.x && positionPrev.y != position.y)
			//{
				x = position.x;
				y = position.y;
				HDC hdc = GetDC(HWND_DESKTOP);
				HBITMAP hbitmap = CreateCompatibleBitmap(hdc, screen_w, screen_h);
				HDC memdc = CreateCompatibleDC(hdc);
				HGDIOBJ oldbmp = SelectObject(memdc, hbitmap);
				BitBlt(memdc, 0, 0, w, h, hdc, x, y, CAPTUREBLT | SRCCOPY);
				SelectObject(memdc, oldbmp);

				BITMAPINFOHEADER infohdr = { sizeof(infohdr), w, h, 1, 32 };
				int size = w * h * 4;
				std::vector<BYTE> bits(size);
				int res = GetDIBits(hdc, hbitmap, 0, h, &bits[0],
					(BITMAPINFO*)&infohdr, DIB_RGB_COLORS);
				if (res != h)
				{
					std::cout << "error\n";
					return 0;
				}

				BYTE* ptr = bits.data();

				for (y = h - 1; y >= 0; y--) //bitmaps bits start from bottom, not top
				{
					for (x = 0; x < w; x++)
					{
						BYTE blu = ptr[0];
						BYTE grn = ptr[1];
						BYTE red = ptr[2];
						ptr += 4;
						_blu = blu;
						_grn = grn;
						_red = red;
					}
				}
				system("cls");
				std::cout << "X:" << position.x << "Y:" << position.y << "\nblu: " << (int)_blu << "\ngrn: " << (int)_grn << "\nred: " << (int)_red;
				positionPrev = position;

				SelectObject(memdc, oldbmp);
				DeleteObject(hbitmap);
				ReleaseDC(HWND_DESKTOP, hdc);
				DeleteDC(memdc);
			//}
		}
	}
	return 0;
}