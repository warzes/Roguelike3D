шахматный шейдер
(в пиксельном отрисываются только четные пиксели (белые), а нечетные (черные) интерполируются из соседей)
#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D Texture;

// Размер текстуры или экрана
uniform vec2 resolution;

// Функция для определения, является ли пиксель "белым" или "черным"
bool isWhitePixel(vec2 fragCoord) {
    return mod(floor(fragCoord.x) + floor(fragCoord.y), 2.0) == 0.0;
}

void main() {
    // Получаем координаты фрагмента в пикселях
    vec2 fragCoord = gl_FragCoord.xy;

    if (isWhitePixel(fragCoord)) {
        // Если пиксель белый, просто отрисовываем его
        FragColor = texture(Texture, TexCoord);
    } else {
        // Если пиксель черный, интерполируем значение из соседних белых пикселей
        vec2 pixelSize = 1.0 / resolution;

        // Соседние белые пиксели
        vec2 offset1 = vec2(-pixelSize.x, -pixelSize.y); // Лево-вверх
        vec2 offset2 = vec2(pixelSize.x, -pixelSize.y);  // Право-вверх
        vec2 offset3 = vec2(-pixelSize.x, pixelSize.y);  // Лево-вниз
        vec2 offset4 = vec2(pixelSize.x, pixelSize.y);   // Право-вниз

        // Берем значения из соседних белых пикселей
        vec4 color1 = texture(Texture, TexCoord + offset1);
        vec4 color2 = texture(Texture, TexCoord + offset2);
        vec4 color3 = texture(Texture, TexCoord + offset3);
        vec4 color4 = texture(Texture, TexCoord + offset4);

        // Интерполяция значений (простой средний)
        FragColor = (color1 + color2 + color3 + color4) * 0.25;
    }
}