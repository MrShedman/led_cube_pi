#include "rate.h"

Rate::Rate(const double hz)
:
m_loop_time(hertz(hz)),
m_loop_clock()
{

}

void Rate::sleep()
{
    ::sleep(m_loop_time - m_loop_clock.get_elapsed_time());
    m_loop_clock.restart();
}