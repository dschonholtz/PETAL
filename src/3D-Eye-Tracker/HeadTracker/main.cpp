#include "head_tracker.h"

int main()
{
    HeadTracker tracker(false, false, tag36h11, 1, 4, 1.0, 0.0, true, false, false);

    while (true)
    {
        tracker.updatePosition();

        if (cv::waitKey(30) >= 0)
        {
            break;
        }
    }

    return 0;
}