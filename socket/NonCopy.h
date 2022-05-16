#ifndef __NONCOPY_H__
#define __NONCOPY_H__

namespace reactor
{

    class NonCopy
    {
    protected:
        NonCopy() {}
        ~NonCopy() {}

    private:
        NonCopy(const NonCopy &);
        NonCopy &operator=(const NonCopy &);
    };

} //
#endif