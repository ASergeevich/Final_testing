 
#include <iostream>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>

static const size_t g_number_of_students {6};

struct Student
{
    size_t m_id;
    std::string m_name;
    size_t m_age;

    Student(size_t id, std::string name, size_t age): m_id {id}, m_name {name}, m_age {age}
    {
    }
};

class StudentBase
{
    std::vector<std::shared_ptr<Student>> m_students;
    std::mutex m_mtx;

    // Возврат итератора на который указывает элемент для удаления
    std::vector<std::shared_ptr<Student>>::iterator get_iterator(size_t id)
    {
        for(auto iter = m_students.begin(); iter < m_students.end(); ++iter)
        {
            if (iter->get()->m_id == id)
            {
                return iter;
            }
        }

        return m_students.end();
    }

    public:

    // Добавление студентов
    void add_information(size_t id, const std::string &name, size_t age)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_students.emplace_back(std::make_shared<Student>(std::move(Student {id, name, age})));
    }

    // Получение инфо по студенту

    std::shared_ptr<Student> get_information(size_t id)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        for(const auto &element: m_students)
        {
            if(element->m_id == id)
            {
                return element;
            }
        }

        return nullptr; // если не найден то возвратим nullptr
    }

    // Удаление студента

    void remove_information(size_t id)
    {

        std::lock_guard<std::mutex> lock(m_mtx);
        auto iter = get_iterator(id);

        if(iter != m_students.end())
        {
            m_students.erase(iter);
        }
        else
        {
            return;
        }
    }
};

void writer(StudentBase &base)
{
    for (auto i{0}; i < g_number_of_students; ++i) {
        base.add_information(i, "Student " + std::to_string('A' + i), 17 + i);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void reader(StudentBase &base) {
    for (auto i{0}; i < g_number_of_students; ++i)
    {
        std::shared_ptr<Student> student = base.get_information(i);
        if (student != nullptr)
        {
            std::cout << "Student ID: " << student->m_id << ", Name: " << student->m_name << ", Age: " << student->m_age << '\n';
        }
        else
        {
            std::cout << "Student with ID " << i << " not found.\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}



int main()
{
    StudentBase base;

    std::thread writerThread(writer, std::ref(base));
    std::thread readerThread(reader, std::ref(base));

    writerThread.join();
    readerThread.join();

    return 0;
}
