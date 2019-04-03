#include <uservga.h>

void User_VGASetPixel(const Vector2 position)
{
	asm volatile("movl $0, %%eax\n\t"
				 "movl %0, %%ebx\n\t"
				 "int $0x81\n"
				 : : "b"(position)
				);
}

void User_VGASetColour(const uint8_t colour)
{
    asm volatile("movl $6, %%eax\n\t"
                 "movzx %0, %%ebx\n\t"
                 "int $0x81\n"
                : : "b"(colour)
            );
}

void User_VGASetLineThickness(const uint8_t thickness)
{
    asm volatile("movl $7, %%eax\n\t"
                 "movzx %0, %%ebx\n\t"
                 "int $0x81\n"
            : : "b"(thickness)
        );
}

void User_VGADrawLine(const Vector2 start, const Vector2 end, const LineType type)
{
	asm volatile("movl $1, %%eax\n\t"
                 "mov %0, %%edx\n\t"
				 "mov %1, %%ecx\n\t"
				 "movl %2, %%ebx\n\t"
				 "int $0x81\n"
				 : : "d"(type), "c"(end), "b"(start)
				);
}

void User_VGADrawRectangle(const Vector2 topLeft, const RectangleDimensions dimensions, const bool fill)
{
	asm volatile("movl $2, %%eax\n\t"
                 "movzx %0, %%edx\n\t"
				 "mov %1, %%ecx\n\t"
				 "mov %2, %%ebx\n\t"
				 "int $0x81\n"
				 : : "d"(fill), "c"(dimensions), "b"(topLeft)
				);
}

void User_VGAClearScreen()
{
    asm volatile("movl $3, %%eax\n\t"
                 "int $0x81\n"
                : :
            );
}

void User_VGADrawCircle(const Vector2 centre, const uint16_t radius, const bool fill)
{
	asm volatile("movl $4, %%eax\n\t"
                 "movzx %0, %%edx\n\t"
				 "movzx %1, %%ecx\n\t"
				 "mov %2, %%ebx\n\t"
				 "int $0x81\n"
				 : : "d"(fill), "c"(radius), "b"(centre)
				);
}

void User_VGADrawPolygon(const Vector2* vertices, const size_t vertexCount, const bool fill)
{
	asm volatile("movl $5, %%eax\n\t"
                 "movzx %0, %%edx\n\t"
				 "movl %1, %%ecx\n\t"
				 "leal (%2), %%ebx\n\t"
				 "int $0x81\n"
				 : : "d"(fill), "c"(vertexCount), "b"(vertices)
				);
}