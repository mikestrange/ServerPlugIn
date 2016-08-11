#include "fd_list.h"


FdList::FdList()
{
    INIT_FDS();
}

void FdList::INIT_FDS()
{
    for(int i = 0; i < MAX_CONNECTS; i++)
    {
        fd_list[i] = {-1, 0};   // connectors.
    }
}

int FdList::maxfds()
{
    return MAX_CONNECTS;
}
    
bool FdList::NEW_FD(int fd, fd_set* fdset, struct sockaddr_in& client_address)
{
    for(int i = 0; i < maxfds(); i++)
    {
        if(fd_list[i].isOn)
        {
            fd_list[i] = {fd, 1, client_address};
            FD_SET(fd, fdset);
            return true;
        }
    }
    return false;
}

bool FdList::REMOVE_FD(int p, fd_set* fdset)
{
    if(fd_list[p].isOn||fd_list[p].fd <= 0) return false;
    //关闭socket
    int fd = fd_list[p].fd;
    fd_list[p].isOn = 0;
    fd_list[p].fd = -1;
    FD_CLR(fd, fdset);
    UIZ::CLOSE(fd);
    return true;
}

int FdList::RESET_FDS(fd_set* fdset)
{
    int max_fd = 0;
    for(int i = 0; i < maxfds(); i++)
    {
        if(fd_list[i].isOn)
        {
            int fd = fd_list[i].fd;
            FD_SET(fd, fdset);
            if(max_fd < fd) max_fd = fd;
        }
    }
    return max_fd;
}

int FdList::ISON_FD(int p, fd_set* fdset)
{
    int fd = fd_list[p].fd;
    if(fd_list[p].isOn && fd > 0 && FD_ISSET(fd, fdset))
    {
        return fd;
    }
    return -1;
}

//关闭而已
void FdList::PUSH_CLOSE(int fd)
{
    for(int i = 0; i < maxfds(); i++)
    {
        if(fd > 0 && fd == fd_list[i].fd)
        {
            fd_list[i].isOn = 0;
            break;
        }
    }
}

void FdList::CLEAN_FDS()
{
    for(int i = 0; i < MAX_CONNECTS; i++)
    {
        fd_list[i].isOn = 0;
        fd_list[i].fd = -1;
    }
}

void FdList::toString()
{
    trace("##server toString begin:");
    for(int i = 0; i < maxfds(); i++)
    {
        if(fd_list[i].isOn)
        {
            trace("on line fd = %d", fd_list[i].fd);
        }
    }
    trace("##server toString end");
}