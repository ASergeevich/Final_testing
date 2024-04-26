#include <iostream>

int main()
{
    const size_t length {10};
    int array[length];

    for(auto i{0}; i < length; ++i)
    {
        array[i] = i + 1;
    }

    std::cout << "Массив элементов: \n";

    for(auto i{0}; i < length; ++i)
    {
        std::cout << array[i] << ' ';
    }

    std::cout << '\n';

    try
    {
        // Устанавливаем указатель на первый элемент
        int *ptr = &array[0];

        // Проверяем если что-то пошло не так выбрасываем исключение
        if (ptr < &array[0] || ptr > &array[length - 1])
        {
            throw std::out_of_range("Нарушены границы!");
        }

        std::cout << "Четвертый элемент: " << *(ptr + 3) << '\n';
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "Ошибка: " << e.what() << '\n';
    }

    return 0;
}
