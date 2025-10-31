#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <queue>
#include <set>
#include <tuple>
#include <random>
#include <ctime>
#include <algorithm>
#include <cassert>
#include <cmath>

using namespace std;

/**
 * @struct Bar
 * @brief Представляє один золотий зливок та його орієнтацію.
 * Розміри: w - ширина (поперек конвеєра), l - довжина (вздовж конвеєра), h - висота (вертикально).
 */
struct Bar
{
    double w, l, h;                         // Поточна орієнтація
    double initial_w, initial_l, initial_h; // Початкові розміри
    double volume;

    Bar(double width, double length, double height)
    {
        w = width;
        l = length;
        h = height;

        initial_w = w;
        initial_l = l;
        initial_h = h;

        volume = w * l * h;
    }

    /**
     * @brief Операція 1: повернути зливок так, щоб його верхня грань (w x l) стала передньою (w x h).
     * 'w' залишається незмінною, 'l' і 'h' міняються місцями.
     */
    void op1_TopToFront()
    {
        swap(l, h);
    }

    /**
     * @brief Операція 2: повернути зливок так, щоб його верхня грань (w x l) стала бічною (l x h).
     * 'l' залишається незмінною, 'w' і 'h' міняються місцями.
     */
    void op2_TopToSide()
    {
        swap(w, h);
    }

    /**
     * @brief Операція 3: повернути зливок так, щоб його передня грань (w x h) стала бічною (l x h).
     * 'h' залишається незмінною, 'w' і 'l' міняються місцями.
     */
    void op3_FrontToSide()
    {
        swap(w, l);
    }

    /**
     * @brief Повертає кортеж поточної орієнтації для використання в std::set.
     */
    tuple<double, double, double> getTuple() const
    {
        return make_tuple(w, l, h);
    }
};

/**
 * @class Conveyor
 * @brief Моделює конвеєр та трюм корабля.
 */
class Conveyor
{
private:
    double holdVolume;
    double remainingVolume;
    double windowWidth, windowHeight;
    double totalLoadedVolume;
    double totalDroppedVolume;
    bool quietMode; // Для unit-тестів, щоб приховати вивід

    /**
     * @brief Допоміжна функція для виводу, якщо не в тихому режимі.
     */
    template <typename T>
    void log(const T &msg)
    {
        if (!quietMode)
        {
            cout << msg;
        }
    }

public:
    /**
     * @brief Конструктор
     * @param vol Загальний об'єм трюму.
     * @param winW Ширина вікна трюму.
     * @param winH Висота вікна трюму.
     * @param quiet Режим без виводу (для тестів).
     */
    Conveyor(double vol, double winW, double winH, bool quiet = false)
        : holdVolume(vol), remainingVolume(vol),
          windowWidth(winW), windowHeight(winH),
          totalLoadedVolume(0), totalDroppedVolume(0),
          quietMode(quiet)
    {
        if (!quietMode)
        {
            cout << fixed << setprecision(2);
            cout << "Конвеєр створено.\n";
            cout << "Загальний об'єм трюму: " << holdVolume << "\n";
            cout << "Розміри вікна (Width x Height): " << windowWidth << " x " << windowHeight << "\n";
        }
    }

    // Геттери для unit-тестів
    double getRemainingVolume() const { return remainingVolume; }
    double getTotalLoadedVolume() const { return totalLoadedVolume; }
    double getTotalDroppedVolume() const { return totalDroppedVolume; }

    /**
     * @brief Перевіряє, чи може зливок *взагалі* пройти у вікно.
     * Перевіряє всі 6 можливих орієнтацій (w, h), яких можна досягти.
     */
    bool canBarEverFit(double d1, double d2, double d3)
    {
        double winW = windowWidth;
        double winH = windowHeight;

        // Всі 6 можливих пар (w, h), які можна подати до вікна:
        // (d1, d2), (d1, d3), (d2, d1), (d2, d3), (d3, d1), (d3, d2)

        if (d1 <= winW && d2 <= winH)
            return true; // (w=d1, h=d2)
        if (d1 <= winW && d3 <= winH)
            return true; // (w=d1, h=d3)
        if (d2 <= winW && d1 <= winH)
            return true; // (w=d2, h=d1)
        if (d2 <= winW && d3 <= winH)
            return true; // (w=d2, h=d3)
        if (d3 <= winW && d1 <= winH)
            return true; // (w=d3, h=d1)
        if (d3 <= winW && d2 <= winH)
            return true; // (w=d3, h=d2)

        return false;
    }

    /**
     * @brief Обробляє один зливок, що надходить на конвеєр.
     */
    void processBar(double initial_w, double initial_l, double initial_h)
    {
        Bar bar(initial_w, initial_l, initial_h);
        double barVolume = bar.volume;

        log("----------------------------------------\n");
        log("Обробка зливку: (w=");
        log(bar.initial_w);
        log(", l=");
        log(bar.initial_l);
        log(", h=");
        log(bar.initial_h);
        log("), Об'єм: ");
        log(barVolume);
        log("\n");
        log("Залишок трюму: ");
        log(remainingVolume);
        log("\n");

        // --- Умова 1: Перевірка на можливість проходження ---
        if (!canBarEverFit(bar.initial_w, bar.initial_l, bar.initial_h))
        {
            log("РІШЕННЯ: Скинути (не проходить у вікно в жодній орієнтації).\n");
            totalDroppedVolume += barVolume;
            return;
        }

        // --- Умова 2: Перевірка об'єму ---
        if (barVolume > 0.5 * remainingVolume)
        {
            log("РІШЕННЯ: Скинути (об'єм ");
            log(barVolume);
            log(" > 50% залишку ");
            log(0.5 * remainingVolume);
            log(").\n");
            totalDroppedVolume += barVolume;
            return;
        }

        // --- Пошук шляху для завантаження (BFS) ---
        // Зливок можна завантажити, шукаємо найкоротший шлях
        queue<pair<Bar, string>> q;                 // Черга станів: (Зливок, Шлях)
        set<tuple<double, double, double>> visited; // Відвідані орієнтації

        // Додаємо початковий стан
        q.push({bar, "ПОЧАТОК"});
        visited.insert(bar.getTuple());

        while (!q.empty())
        {
            pair<Bar, string> current = q.front();
            q.pop();
            Bar b = current.first;
            string path = current.second;

            // --- Умова успіху: зливок проходить у вікно ---
            if (b.w <= windowWidth && b.h <= windowHeight)
            {
                log("Знайдено шлях:\n");
                log(path);
                log("\n");
                log("РЕЗУЛЬТАТ: Завантажено (кінцева орієнтація w=");
                log(b.w);
                log(", h=");
                log(b.h);
                log(").\n");
                remainingVolume -= barVolume;
                totalLoadedVolume += barVolume;
                return; // Успішно завантажено
            }

            // --- Дослідження сусідніх станів (операцій) ---

            // Операція 1
            Bar b1 = b;
            b1.op1_TopToFront();
            if (visited.find(b1.getTuple()) == visited.end())
            {
                visited.insert(b1.getTuple());
                q.push({b1, path + " -> OP1(Верх->Перед)"});
            }

            // Операція 2
            Bar b2 = b;
            b2.op2_TopToSide();
            if (visited.find(b2.getTuple()) == visited.end())
            {
                visited.insert(b2.getTuple());
                q.push({b2, path + " -> OP2(Верх->Бік)"});
            }

            // Операція 3
            Bar b3 = b;
            b3.op3_FrontToSide();
            if (visited.find(b3.getTuple()) == visited.end())
            {
                visited.insert(b3.getTuple());
                q.push({b3, path + " -> OP3(Перед->Бік)"});
            }
        }

        // Цей код не мав би виконуватись, якщо canBarEverFit працює коректно
        log("ПОМИЛКА ЛОГІКИ: canBarEverFit=true, але шлях не знайдено.\n");
        totalDroppedVolume += barVolume;
    }

    /**
     * @brief Друкує фінальний звіт про завантаження.
     */
    void printFinalReport()
    {
        if (quietMode)
            return;

        cout << "\n========================================\n";
        cout << "ЗАВАНТАЖЕННЯ ЗАВЕРШЕНО\n";
        cout << "========================================\n";
        cout << "Загальний об'єм трюму: " << holdVolume << "\n";
        cout << "Загальний завантажений об'єм: " << totalLoadedVolume << "\n";
        cout << "Загальний скинутий об'єм: " << totalDroppedVolume << "\n";
        cout << "Залишок вільного місця: " << remainingVolume << "\n";
    }
};

/**
 * @brief Допоміжна функція для генерації випадкового double.
 */
double rand_double(mt19937 &gen, double min, double max)
{
    uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

bool approx_equal(double a, double b)
{
    return abs(a - b) < 1e-9;
}

/**
 * @brief Головна функція програми.
 */
int main()
{
#ifdef RUN_TESTS
    cout << "=== Запуск unit-тестів ===\n";

    // --- Тест 1: Перевірка операцій обертання ---
    {
        Bar b(2, 3, 4);
        double V = b.volume;
        assert(V == 24);

        b.op1_TopToFront(); // міняє l і h → (2,4,3)
        assert(b.w == 2 && b.l == 4 && b.h == 3);

        b.op2_TopToSide(); // міняє w і h → (3,4,2)
        assert(b.w == 3 && b.l == 4 && b.h == 2);

        b.op3_FrontToSide(); // міняє w і l → (4,3,2)
        assert(b.w == 4 && b.l == 3 && b.h == 2);

        assert(approx_equal(b.volume, V));
        cout << "✅ Тест 1 пройдено: операції обертання зберігають об'єм.\n";
    }

    // --- Тест 2: Перевірка canBarEverFit ---
    {
        Conveyor conv(1000, 10, 5, true); // Вікно W=10, H=5, тихий режим

        // Може пройти (3x4), (4x3), (6x3), (6x4)
        assert(conv.canBarEverFit(3, 4, 6) == true);

        // Може пройти (4x3), (3x4)
        assert(conv.canBarEverFit(11, 4, 3) == true);

        // Може пройти (6x3)
        assert(conv.canBarEverFit(3, 6, 11) == true);

        // Не може пройти (жодна комбінація не менша 10x5)
        // (11, 6), (11, 7), (6, 11), (6, 7), (7, 11), (7, 6)
        assert(conv.canBarEverFit(11, 6, 7) == false);

        cout << "✅ Тест 2 пройдено: canBarEverFit працює коректно.\n";
    }

    // --- Тест 3: Перевірка завантаження/скидання  ---
    {
        Conveyor conv(1000, 10, 10, true); // Вікно W=10, H=10, Vol=1000, тихий режим

        // 1. Зливок (5, 5, 5), Vol=125.
        // canFit=true. 125 > 0.5*1000 (false). Проходить (5x5).
        // РІШЕННЯ: Завантажити.
        conv.processBar(5, 5, 5);
        assert(approx_equal(conv.getRemainingVolume(), 875)); // 1000 - 125
        assert(approx_equal(conv.getTotalLoadedVolume(), 125));
        assert(approx_equal(conv.getTotalDroppedVolume(), 0));

        // 2. Зливок (20, 5, 5), Vol=500.
        // canFit=true (можна 5x5). 500 > 0.5*875 (500 > 437.5) (true).
        // РІШЕННЯ: Скинути (через об'єм).
        conv.processBar(20, 5, 5);
        assert(approx_equal(conv.getRemainingVolume(), 875)); // Без змін
        assert(approx_equal(conv.getTotalLoadedVolume(), 125));
        assert(approx_equal(conv.getTotalDroppedVolume(), 500)); // 0 + 500

        // 3. Зливок (11, 11, 11), Vol=1331.
        // canFit=false (найменша 11x11 > 10x10).
        // РІШЕННЯ: Скинути (не проходить).
        conv.processBar(11, 11, 11);
        assert(approx_equal(conv.getRemainingVolume(), 875)); // Без змін
        assert(approx_equal(conv.getTotalLoadedVolume(), 125));
        assert(approx_equal(conv.getTotalDroppedVolume(), 1831)); // 500 + 1331

        // 4. Зливок (5, 5, 5), Vol=125.
        // canFit=true. 125 > 0.5*875 (125 > 437.5) (false). Проходить (5x5).
        // РІШЕННЯ: Завантажити.
        conv.processBar(5, 5, 5);
        assert(approx_equal(conv.getRemainingVolume(), 750));     // 875 - 125
        assert(approx_equal(conv.getTotalLoadedVolume(), 250));   // 125 + 125
        assert(approx_equal(conv.getTotalDroppedVolume(), 1831)); // Без змін

        cout << "✅ Тест 3 пройдено: логіка завантаження/скидання коректна.\n";
    }

    cout << "\nУсі тести пройдено успішно ✅\n";
    return 0;

#else
    // === Звичайний режим програми ===
    mt19937 gen(static_cast<unsigned int>(time(0)));

    // double holdVolume = rand_double(gen, 5000.0, 10000.0); 
    // double windowW = rand_double(gen, 10.0, 30.0); 
    // double windowH = rand_double(gen, 10.0, 30.0); 
    // int numBars = 15; // Кількість зливків для обробки

    double holdVolume, windowW, windowH;
    int numBars;

    cout << "Введіть загальний об'єм трюму: ";
    cin >> holdVolume;
    cout << "Введіть ширину вікна: ";
    cin >> windowW;
    cout << "Введіть висоту вікна: ";
    cin >> windowH;
    cout << "Введіть кількість зливків для обробки: ";
    cin >> numBars;

    Conveyor conveyor(holdVolume, windowW, windowH);

    for (int i = 0; i < numBars; ++i)
    {
        double barW, barL, barH;
        cout << "\n--- Зливок " << (i + 1) << " ---\n";
        cout << "Введіть початкову ширину (w): ";
        cin >> barW;
        cout << "Введіть початкову довжину (l): ";
        cin >> barL;
        cout << "Введіть початкову висоту (h): ";
        cin >> barH;

        conveyor.processBar(barW, barL, barH);
    }

    conveyor.printFinalReport();
    return 0;
#endif
}
