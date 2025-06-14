#include <iostream> // Для ввода/вывода (std::cout, std::endl)
#include <vector>   // Для std::vector (хотя в этом коде не используется напрямую, но часто нужен в подобных задачах)
#include <cmath>    // Для математических функций (std::sqrt, std::pow, std::abs, std::log, M_PI)
#include <iomanip>  // Для форматированного вывода (std::fixed, std::setprecision, std::setw)
#include <functional> // Для std::function (используется для передачи функций как аргументов)
#include <algorithm> // Для std::algorithm (хотя в этом коде не используется напрямую)

// Определение M_PI, если не определено (например, в MinGW)
// M_PI - математическая константа, равная числу π (пи).
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief Определяет подынтегральную функцию.
 * В данном случае, f(x) = (x + 3) / (x^2 + 4).
 * Это функция, интеграл от которой мы хотим численно вычислить.
 * @param x Аргумент функции.
 * @return Значение функции в точке x.
 */
double func(const double x) {
    return (x + 3.0) / (x * x + 4.0);
}

/**
 * @brief Определяет вторую производную подынтегральной функции f''(x).
 * f''(x) = 2 * (x^3 + 9x^2 - 12x - 12) / (x^2 + 4)^3.
 * Используется для теоретического расчета необходимого числа разбиений (шага)
 * в методах, где оценка погрешности зависит от второй производной (например, методы трапеций и центральных прямоугольников).
 * Знание M2 = max|f''(x)| на отрезке [a,b] позволяет оценить погрешность.
 * @param x Аргумент функции.
 * @return Значение второй производной в точке x.
 */
double func_dd(const double x) {
    const double num = 2.0 * (x*x*x + 9.0*x*x - 12.0*x - 12.0); // Числитель второй производной
    const double den_base = (x*x + 4.0); // Основание знаменателя (x^2 + 4)
    // Знаменатель равен (x^2 + 4)^3
    return num / (den_base*den_base*den_base);
}

/**
 * @brief Метод центральных прямоугольников для численного интегрирования.
 *
 * @details
 * Теория:
 * Отрезок интегрирования [a,b] делится на n равных подынтервалов шириной h = (b-a)/n.
 * На каждом подынтервале [x_i, x_{i+1}] функция f(x) аппроксимируется константой,
 * равной значению функции в середине этого подынтервала: f(x_i + h/2).
 * Площадь под кривой на подынтервале заменяется площадью прямоугольника.
 * Формула: I ≈ h * Σ_{i=0}^{n-1} f(a + (i + 0.5)h)
 *
 * Геометрическая интерпретация:
 * Сумма площадей прямоугольников, верхняя сторона которых касается графика функции в средней точке основания.
 *
 * Порядок точности:
 * Второй (O(h^2)). При уменьшении шага h в два раза, ошибка уменьшается примерно в четыре раза.
 *
 * Плюсы:
 * - Простота реализации.
 * - Часто точнее методов левых/правых прямоугольников (которые имеют первый порядок точности).
 *
 * Минусы:
 * - Менее точен, чем методы более высокого порядка (трапеций, Симпсона) при том же n.
 *
 * @param a Нижний предел интегрирования.
 * @param b Верхний предел интегрирования.
 * @param n Количество разбиений (подынтервалов).
 * @return Приближенное значение интеграла.
 */
double central_rectangles(double a, double b, int n) {
    if (n <= 0) return 0.0; // Проверка корректности числа разбиений
    double h = (b - a) / n;   // Ширина одного подынтервала (шаг)
    double sum = 0.0;         // Сумма значений функции в серединах подынтервалов
    for (int i = 0; i < n; ++i) {
        // Вычисляем координату середины i-го подынтервала и значение функции в этой точке
        sum += func(a + (i + 0.5) * h);
    }
    return h * sum; // Результат по формуле центральных прямоугольников
}

/**
 * @brief Метод трапеций для численного интегрирования.
 *
 * @details
 * Теория:
 * Отрезок [a,b] делится на n подынтервалов шириной h = (b-a)/n.
 * На каждом подынтервале [x_i, x_{i+1}] функция f(x) аппроксимируется прямой линией (линейной функцией),
 * проходящей через точки (x_i, f(x_i)) и (x_{i+1}, f(x_{i+1})).
 * Площадь под кривой на подынтервале заменяется площадью трапеции.
 * Формула: I ≈ h * [ (f(a) + f(b))/2 + Σ_{i=1}^{n-1} f(a + ih) ]
 *
 * Геометрическая интерпретация:
 * Сумма площадей трапеций, верхние стороны которых являются хордами,
 * соединяющими значения функции на концах каждого подынтервала.
 *
 * Порядок точности:
 * Второй (O(h^2)). Такой же, как у метода центральных прямоугольников.
 *
 * Плюсы:
 * - Относительно прост в реализации.
 * - Интуитивно понятная аппроксимация.
 *
 * Минусы:
 * - Та же точность, что и у центральных прямоугольников. Константа в оценке погрешности
 *   может быть как больше, так и меньше в зависимости от функции.
 *
 * @param a Нижний предел интегрирования.
 * @param b Верхний предел интегрирования.
 * @param n Количество подынтервалов (узлов n+1).
 * @return Приближенное значение интеграла.
 */
double trapezoidal_rule(const double a, const double b, const int n) {
    if (n <= 0) return 0.0; // Проверка корректности числа разбиений
    const double h = (b - a) / n; // Ширина одного подынтервала (шаг)
    // Начальное значение суммы: полусумма значений функции на концах отрезка [a,b],
    // что соответствует первому и последнему слагаемому в формуле с коэффициентом 1/2.
    double sum = (func(a) + func(b)) / 2.0;
    // Суммируем значения функции во внутренних узлах (с коэффициентом 1)
    for (int i = 1; i < n; ++i) {
        sum += func(a + i * h);
    }
    return h * sum; // Результат по формуле трапеций
}

/**
 * @brief Метод Симпсона (формула парабол) для численного интегрирования.
 *
 * @details
 * Теория:
 * Отрезок [a,b] делится на ЧЕТНОЕ число n подынтервалов шириной h = (b-a)/n.
 * На каждой паре смежных подынтервалов (т.е. на отрезке длиной 2h) функция f(x)
 * аппроксимируется параболой (квадратичной функцией), проходящей через три точки.
 * Площадь под кривой на этом двойном подынтервале заменяется площадью под параболой.
 * Формула (составная): I ≈ (h/3) * [ f(x₀) + 4f(x₁) + 2f(x₂) + 4f(x₃) + ... + 2f(x_{n-2}) + 4f(x_{n-1}) + f(x_n) ]
 *
 * Геометрическая интерпретация:
 * Сумма площадей под параболическими сегментами, которые аппроксимируют
 * график функции на парах подынтервалов.
 *
 * Порядок точности:
 * Четвертый (O(h^4)). Значительно выше, чем у методов прямоугольников и трапеций.
 * При уменьшении шага h в два раза, ошибка уменьшается примерно в 16 раз.
 * Метод точен для многочленов до третьей степени включительно.
 *
 * Плюсы:
 * - Высокая точность при сравнительно небольшом n (для гладких функций).
 * - Очень эффективен для большинства практических задач.
 *
 * Минусы:
 * - Требует четного числа подынтервалов n.
 * - Немного сложнее в реализации.
 *
 * @param a Нижний предел интегрирования.
 * @param b Верхний предел интегрирования.
 * @param n Количество подынтервалов (должно быть четным). Если n нечетное, оно будет увеличено на 1.
 * @return Приближенное значение интеграла.
 */
double simpsons_rule(const double a, const double b, int n) {
    if (n <= 0) return 0.0; // Проверка корректности числа разбиений
    // Метод Симпсона требует четного числа подынтервалов n.
    // Если n нечетное, увеличиваем его до ближайшего четного.
    // Это важно, так как правило Рунге может передать нечетное n на начальных итерациях.
    if (n % 2 != 0) {
        n++;
    }

    const double h = (b - a) / n; // Ширина одного подынтервала (шаг)
    // Начальное значение суммы: значения функции на концах отрезка [a,b] (коэффициенты 1)
    double sum = func(a) + func(b);

    // Суммируем значения функции во внутренних узлах с соответствующими коэффициентами (4 или 2)
    for (int i = 1; i < n; ++i) {
        if (i % 2 == 1) { // Нечетные индексы (x_1, x_3, ...) имеют коэффициент 4
            sum += 4 * func(a + i * h);
        } else { // Четные индексы (x_2, x_4, ...) имеют коэффициент 2
            sum += 2 * func(a + i * h);
        }
    }
    return (h / 3.0) * sum; // Результат по формуле Симпсона
}

/**
 * @brief Адаптивный метод численного интегрирования с использованием правила Рунге и уточнения по Ричардсону.
 *
 * @details
 * Смысл и теория:
 * Этот подход не требует знания производных функции. Он основан на сравнении результатов,
 * полученных с разным шагом, для оценки погрешности и адаптивного выбора шага.
 *
 * 1. Правило Рунге для оценки погрешности:
 *    - Вычисляется интеграл I_h с текущим шагом h (число разбиений n).
 *    - Вычисляется интеграл I_{2h} с шагом 2h (число разбиений n/2). В реализации I_{2h} - это I_h с предыдущей итерации.
 *    - Если основной метод численного интегрирования имеет порядок точности p, то погрешность I_h
 *      можно оценить как: R_h ≈ (I_h - I_{2h}) / (2^p - 1).
 *    - Итерационный процесс: начинаем с малого n. Если оценка |R_h| больше заданной точности epsilon,
 *      удваиваем n (уменьшаем шаг h вдвое) и повторяем вычисления.
 *
 * 2. Уточнение по Ричардсону (экстраполяция Ричардсона):
 *    - Используя оценку погрешности R_h, можно получить более точное значение интеграла:
 *      I_уточн = I_h + R_h = I_h + (I_h - I_{2h}) / (2^p - 1).
 *    - Это уточненное значение обычно имеет более высокий порядок точности, чем исходный метод.
 *
 * Как решает задачу:
 * Адаптивно подбирает шаг h (число разбиений n) "на лету", пока не будет достигнута
 * требуемая точность epsilon. При этом также позволяет улучшить итоговый результат.
 *
 * Плюсы:
 * - Не требует вычисления производных подынтегральной функции.
 * - Адаптируется к поведению функции (хотя в данной реализации шаг глобальный, но число n подбирается).
 * - Уточнение по Ричардсону часто дает значительный прирост точности.
 * - Более универсален, чем методы, требующие M2.
 *
 * Минусы:
 * - Выполняет вычисления интеграла несколько раз (минимум дважды для каждой итерации подбора шага).
 * - Оценка погрешности по Рунге является асимптотической (хорошо работает при достаточно малых h).
 * - Для методов высокого порядка (как Симпсон), правило Рунге становится надежным при достаточно большом n,
 *   чтобы главная часть погрешности действительно доминировала (поэтому используется min_n_for_reliable_runge).
 *
 * @param integrator Функция, реализующая один из методов численного интегрирования (например, trapezoidal_rule, simpsons_rule).
 * @param a Нижний предел интегрирования.
 * @param b Верхний предел интегрирования.
 * @param epsilon Требуемая точность.
 * @param p Порядок точности метода, используемого в 'integrator' (2 для трапеций, 4 для Симпсона).
 * @param n_final Ссылка для возврата итогового количества разбиений, при котором была достигнута точность.
 * @return Уточненное значение интеграла.
 */
double integrate_with_runge(const std::function<double(double, double, int)> &integrator,
                            const double a, const double b, double epsilon, const int p, int& n_final) {
    // Начальное количество разбиений.
    // Обычно начинают с малого числа, например, 2.
    int n = 2;
    // Для метода Симпсона (p=4) n должно быть четным.
    // Если начальное n нечетное, делаем его четным.
    // Также, если начальное n слишком мало (например, 0 или 1), устанавливаем минимальное четное n=2.
    if (p == 4) {
        n = (n % 2 == 0) ? n : n + 1;
        if (n == 0) n = 2;
    }


    double I_h = integrator(a, b, n); // Интеграл с текущим шагом h (соответствует n разбиениям)
    double I_2h;                      // Интеграл с шагом 2h (соответствует n/2 разбиениям) - будет значением I_h с предыдущей итерации

    // Максимальное количество итераций для предотвращения зацикливания
    // если точность не достигается (например, из-за особенностей функции или слишком жесткой epsilon).
    constexpr int max_iterations = 2000;
    int current_iteration = 0;

    // Минимальное количество разбиений n для надежной оценки погрешности по правилу Рунге,
    // особенно для методов высокого порядка точности (как Симпсон, p=4).
    // При малых n оценка Рунге может быть нестабильной или неточной.
    int min_n_for_reliable_runge = 1; // По умолчанию
    if (p == 4) {
        // Для Симпсона (p=4) эмпирически выбирают n >= 4 или n >= 8.
        // В отчете для Симпсона n начинается с 4, итоговое n=8 (после удвоения).
        // Здесь n начинается с 2 (или 4 если было нечетное), потом 4, 8...
        min_n_for_reliable_runge = 8;
    }


    do {
        I_2h = I_h; // Сохраняем предыдущее значение интеграла (это I_h для предыдущего n, т.е. I_2h для текущего удвоенного n)
        n *= 2;     // Удваиваем количество разбиений (уменьшаем шаг h вдвое)
        I_h = integrator(a, b, n); // Вычисляем интеграл с новым, меньшим шагом

        // Оценка погрешности по правилу Рунге: R_h ≈ (I_h - I_2h) / (2^p - 1)
        double error_estimate_component = I_h - I_2h; // Числитель в оценке Рунге
        double runge_denominator = std::pow(2, p) - 1.0;
        double current_error_estimate = std::abs(error_estimate_component) / runge_denominator;

        // Условие остановки итерационного процесса:
        bool stopping_condition_met = (current_error_estimate < epsilon);

        // Дополнительная проверка для методов высокого порядка (как Симпсон):
        // Продолжаем итерации, если n еще слишком мало для надежной оценки по Рунге,
        // даже если формальная оценка погрешности мала. Это предотвращает преждевременную остановку
        // на нестабильных оценках при малых n.
        if (p >= 4 && n < min_n_for_reliable_runge && stopping_condition_met && current_iteration < max_iterations -1 ) {
            stopping_condition_met = false; // Форсируем продолжение итераций
        }


        if (stopping_condition_met) {
            n_final = n; // Сохраняем итоговое количество разбиений
            // Возвращаем уточненное значение по Ричардсону: I_уточн = I_h + (I_h - I_2h) / (2^p - 1)
            // (I_h - I_2h) / (2^p - 1)  это error_estimate_component / runge_denominator (со знаком)
            return I_h + (error_estimate_component / runge_denominator);
        }

        current_iteration++;
        // Предохранитель от слишком большого числа разбиений (и слишком долгого вычисления)
        if (n > 4000000) { // Порог можно настроить
             std::cerr << "Предупреждение: достигнуто максимальное количество разбиений n=" << n
                       << " для правила Рунге. Точность может быть не достигнута." << std::endl;
             break; // Выход из цикла, если n становится слишком большим
        }

    } while (current_iteration < max_iterations);

    // Если цикл завершился по max_iterations или по n > 4000000, а не по достижению точности
    std::cerr << "Предупреждение: правило Рунге не сошлось за " << current_iteration
              << " итераций (или n стало слишком большим) до заданной точности epsilon=" << epsilon
              << ". Последняя оценка погрешности: "
              << (std::abs(I_h - I_2h) / (std::pow(2, p) - 1.0)) << std::endl;
    n_final = n;
    // Возвращаем лучшее из имеющихся значений. Можно вернуть I_h или уточненное,
    // если оно считается более надежным даже при неполной сходимости.
    // В данном случае, если не сошлось, возвращаем последнее вычисленное I_h или его уточнение.
    return I_h + (I_h - I_2h) / (std::pow(2,p) - 1.0); // Возвращаем последнее уточненное значение
}

int main() {
    // Настройка вывода чисел с плавающей точкой: фиксированный формат, 8 знаков после запятой
    std::cout << std::fixed << std::setprecision(8);

    // Исходные данные задачи
    double a = 0.0; // Нижний предел интегрирования
    double b = 2.0; // Верхний предел интегрирования
    // Заданная точность epsilon. Для методов с M2 она используется для расчета n.
    // Для методов с правилом Рунге это целевая погрешность.
    double epsilon = 0.0001;

    // Вывод заголовка и исходных данных
    std::cout << "Вычислительная математика - Домашнее задание N4" << std::endl;
    std::cout << "Методы численного интегрирования" << std::endl;
    std::cout << "Функция: f(x) = (x+3) / (x^2+4)" << std::endl;
    std::cout << "Интервал: [" << a << ", " << b << "]" << std::endl;
    std::cout << "Заданная точность epsilon = " << epsilon << std::endl << std::endl;

    // 1. Вычисление точного значения интеграла по формуле Ньютона-Лейбница
    //    Первообразная F(x) = (1/2)ln(x^2+4) + (3/2)arctan(x/2).
    //    Интеграл I = F(b) - F(a) = F(2) - F(0).
    //    F(2) = (1/2)ln(8) + (3/2)arctan(1) = (1/2)ln(8) + (3/2)(π/4) = (1/2)ln(8) + 3π/8.
    //    F(0) = (1/2)ln(4) + (3/2)arctan(0) = (1/2)ln(4).
    //    I = (1/2)ln(8) - (1/2)ln(4) + 3π/8 = (1/2)ln(8/4) + 3π/8 = (1/2)ln(2) + 3π/8.
    //    Это эталонное значение для сравнения точности численных методов.
    const double exact_value = (0.5 * std::log(2.0)) + (3.0 * M_PI / 8.0);

    std::cout << "1. Точное значение интеграла (Ньютон-Лейбниц): " << exact_value << std::endl << std::endl;

    // 2. Методы с выбором шага на основе оценки второй производной (M2)
    //    Теоретический подход: если известна M2 = max|f''(x)| на [a,b], можно
    //    заранее рассчитать число разбиений n, необходимое для достижения заданной точности epsilon.
    //    Формулы погрешности:
    //    - Для центральных прямоугольников: |Rn| <= (b-a)^3 * M2 / (24 * n^2)
    //    - Для трапеций: |Rn| <= (b-a)^3 * M2 / (12 * n^2)
    //    Отсюда n >= sqrt((b-a)^3 * M2 / (k * epsilon)), где k=24 или k=12.
    //
    //    Плюсы подхода: теоретическая гарантия (если M2 точна).
    //    Минусы: требует нахождения f''(x) и ее максимума, что может быть сложно.
    //            Оценка M2 может быть грубой, приводя к избыточному n.
    std::cout << "2. Вычисление интеграла по формуле центральных прямоугольников и трапеции" << std::endl;
    std::cout << "   с выбором шага (вторая производная M2) для удовлетворения заданной точности epsilon=" << epsilon << ":" << std::endl;

    // M2 - максимальное значение модуля второй производной |f''(x)| на [a,b].
    // Из отчета (стр. 7), M2 ≈ 0.431563 в точке x ≈ 0.296137. Используем значение 0.43156.
    constexpr double M2_val = 0.43156;
    std::cout << "   Используемое значение max|f''(x)| = M2 = " << M2_val << std::endl;

    // Расчет n для метода центральных прямоугольников по M2
    int n_rect_m2 = static_cast<int>(std::ceil(std::sqrt(std::pow(b - a, 3) * M2_val / (24.0 * epsilon))));
    if (n_rect_m2 == 0) n_rect_m2 = 1; // n должно быть хотя бы 1
    const double h_rect_m2 = (b - a) / n_rect_m2;
    const double res_central_rect_m2 = central_rectangles(a, b, n_rect_m2);
    std::cout << "   Метод центральных прямоугольников (на основе M2):" << std::endl;
    std::cout << "     Рассчитанное количество разбиений n = " << n_rect_m2 << " (из отчета: 38)" << std::endl;
    std::cout << "     Фактический шаг h = " << h_rect_m2 << std::endl;
    std::cout << "     Результат = " << res_central_rect_m2
            << ", Абс. погрешность = " << std::abs(res_central_rect_m2 - exact_value) << std::endl;

    // Расчет n для метода трапеций по M2
    int n_trap_m2 = static_cast<int>(std::ceil(std::sqrt(std::pow(b - a, 3) * M2_val / (12.0 * epsilon))));
    if (n_trap_m2 == 0) n_trap_m2 = 1; // n должно быть хотя бы 1
    const double h_trap_m2 = (b - a) / n_trap_m2;
    const double res_trap_m2 = trapezoidal_rule(a, b, n_trap_m2);
    std::cout << "   Метод трапеций (на основе M2):" << std::endl;
    std::cout << "     Рассчитанное количество разбиений n = " << n_trap_m2 << " (из отчета: 54)" << std::endl;
    std::cout << "     Фактический шаг h = " << h_trap_m2 << std::endl;
    std::cout << "     Результат = " << res_trap_m2
            << ", Абс. погрешность = " << std::abs(res_trap_m2 - exact_value) << std::endl << std::endl;

    // 3. Методы с автоматическим выбором шага по правилу Рунге
    //    Практический адаптивный подход, не требующий знания производных.
    //    Итерационно подбирает n, пока оценка погрешности по Рунге не станет меньше epsilon.
    //    Результат уточняется по Ричардсону.
    std::cout << "3. Вычисление интеграла по формуле трапеций и Симпсона" << std::endl;
    std::cout << "   с автоматическим выбором шага по правилу Рунге для удовлетворения заданной точности epsilon=" << epsilon << ":" << std::endl;

    int n_trap_runge_final; // Для сохранения итогового n
    // p=2 для метода трапеций (порядок точности)
    const double res_trap_runge = integrate_with_runge(trapezoidal_rule, a, b, epsilon, 2, n_trap_runge_final);
    std::cout << "   Метод трапеций (Рунге, p=2):" << std::endl;
    std::cout << "     Итоговое количество разбиений n = " << n_trap_runge_final << " (из отчета: 16, но после уточнения, может быть 64)" << std::endl;
    std::cout << "     Результат = " << res_trap_runge
            << ", Абс. погрешность = " << std::abs(res_trap_runge - exact_value) << std::endl;

    int n_simpson_runge_final; // Для сохранения итогового n
    // p=4 для метода Симпсона (порядок точности)
    const double res_simpson_runge = integrate_with_runge(simpsons_rule, a, b, epsilon, 4, n_simpson_runge_final);
    std::cout << "   Метод Симпсона (Рунге, p=4):" << std::endl;
    std::cout << "     Итоговое количество разбиений n = " << n_simpson_runge_final << " (из отчета: 4, но после уточнения, может быть 8)" << std::endl;
    std::cout << "     Результат = " << res_simpson_runge
            << ", Абс. погрешность = " << std::abs(res_simpson_runge - exact_value) << std::endl << std::endl;

    // 4. Сравнение полученных результатов с точным значением
    //    Таблица для наглядного представления точности и эффективности методов.
    std::cout << "4. Сравнение полученных результатов с точным значением (" << exact_value << "):" << std::endl;
    std::cout << "   ---------------------------------------------------------------------------------" << std::endl;
    std::cout << "   | Метод                           | N итог. | Результат        | Абсолютная погрешность   |" << std::endl;
    std::cout << "   |---------------------------------|----------|------------------|--------------------------|" << std::endl;
    std::cout << "   | Центр. прямоугольники (M2)      | " << std::setw(8) << n_rect_m2 << " | " << std::setw(16) << res_central_rect_m2 << " | " <<
            std::setw(24) << std::abs(res_central_rect_m2 - exact_value) << " |" << std::endl;
    std::cout << "   | Трапеции (M2)                   | " << std::setw(8) << n_trap_m2 << " | " << std::setw(16) << res_trap_m2 << " | " << std::setw(24) <<
            std::abs(res_trap_m2 - exact_value) << " |" << std::endl;
    std::cout << "   | Трапеции (Рунге)                | " << std::setw(8) << n_trap_runge_final << " | " << std::setw(16) << res_trap_runge << " | " << std::setw(24) <<
            std::abs(res_trap_runge - exact_value) << " |" << std::endl;
    std::cout << "   | Симпсон (Рунге)                 | " << std::setw(8) << n_simpson_runge_final << " | " << std::setw(16) << res_simpson_runge << " | " << std::setw(24)
            << std::abs(res_simpson_runge - exact_value) <<  " |" << std::endl;
    std::cout << "   ---------------------------------------------------------------------------------" << std::endl <<
            std::endl;

    // Проверка, все ли методы достигли заданной точности epsilon.
    // Для M2-методов, epsilon используется для расчета n, фактическая погрешность может немного отличаться.
    // Для Рунге-методов, epsilon - это целевая точность, и фактическая погрешность должна быть <= epsilon.
    bool all_accurate_flag = true;
    if (std::abs(res_central_rect_m2 - exact_value) > epsilon) {
        std::cout << "Предупреждение: Погрешность метода центральных прямоугольников (M2) ("
                  << std::abs(res_central_rect_m2 - exact_value) << ") > epsilon (" << epsilon << ")." << std::endl;
        all_accurate_flag = false;
    }
    if (std::abs(res_trap_m2 - exact_value) > epsilon) {
        std::cout << "Предупреждение: Погрешность метода трапеций (M2) ("
                  << std::abs(res_trap_m2 - exact_value) << ") > epsilon (" << epsilon << ")." << std::endl;
        all_accurate_flag = false;
    }
    if (std::abs(res_trap_runge - exact_value) > epsilon) {
        std::cout << "Предупреждение: Погрешность метода трапеций (Рунге) ("
                  << std::abs(res_trap_runge - exact_value) << ") > epsilon (" << epsilon << ")." << std::endl;
        all_accurate_flag = false;
    }
    if (std::abs(res_simpson_runge - exact_value) > epsilon) {
        std::cout << "Предупреждение: Погрешность метода Симпсона (Рунге) ("
                  << std::abs(res_simpson_runge - exact_value) << ") > epsilon (" << epsilon << ")." << std::endl;
        all_accurate_flag = false;
    }

    if (all_accurate_flag) {
        std::cout << "Все методы, для которых epsilon=" << epsilon << " является целевой точностью, достигли ее." << std::endl;
        std::cout << "Для M2-методов, epsilon=" << epsilon << " использовался для расчета n, фактическая погрешность может немного отличаться." << std::endl;
    } else {
        std::cout << "Не все методы достигли целевой точности epsilon=" << epsilon << " (см. предупреждения выше)." << std::endl;
    }

    return 0;
}