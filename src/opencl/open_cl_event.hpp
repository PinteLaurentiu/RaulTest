//
// Created by laurentiu on 04.07.2020.
//

#ifndef RAULTESTQT_OPEN_CL_EVENT_HPP
#define RAULTESTQT_OPEN_CL_EVENT_HPP


#include <memory>
#include <CL/cl.h>

void deleteOpenClEvent(_cl_event*);

using OpenCLEvent = std::shared_ptr<_cl_event>;


#endif //RAULTESTQT_OPEN_CL_EVENT_HPP
