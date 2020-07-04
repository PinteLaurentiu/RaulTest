//
// Created by laurentiu on 04.07.2020.
//

#include "open_cl_event.hpp"

void deleteOpenClEvent(_cl_event* event) {
    if (!event)
        return;
    clReleaseEvent(event);
}
