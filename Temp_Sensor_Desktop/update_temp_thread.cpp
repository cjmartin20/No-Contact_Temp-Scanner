#include "update_temp_thread.h"


class update_temp_thread : public QThread
{
    Q_OBJECT

protected:
    void run();
};

void update_temp_thread::run()
{
    while(1){
        printf("test");
    }
}
