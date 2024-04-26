#include <thread>
#include <shared_mutex>
#include <iostream>

int variable = 0;
//используем фичи c++17
std::shared_mutex mtx;

void ChangeVariableMtx() {
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        int isPositive = variable >= 0;
        std::lock_guard lock(mtx);
        variable  = variable + 5*!isPositive - 1*isPositive;
    }
}
void ReadVariableMtx() {
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        //блокируем только в случае, если нет писателей
        std::shared_lock lock(mtx);
        std::cout << variable << std::endl;
    }
}

int main(void) {
    std::thread changer(ChangeVariableMtx);
    std::thread reader(ReadVariableMtx);

    changer.join();
    changer.join();
}