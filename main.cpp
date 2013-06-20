#include "main_skeleton.h"
#include "params.h"

#include "detector.h"


/**********************************************
            This whole code is based on the code of tree other persons

            i) Zdenek Kalal
                Creator of the TLD Predator, who published an implementation called OpenTLD.
            ii) Bernd Lukatschek and Georg Nebehay
                The guys who successfully implemented his own version of the TLD based on the original OpenTLD
            iii) Ben Pryke
                This guy also implemented a version of the TLD Alg.
                but it doesn`t work as good as the former implementations cited,
                mainly because of its changes on the actual algorithm, I believe.
                However, this imp. was very useful to get the first insight about the functioning of the TLD aka Pretador.

            My main purpose was to learn how the algorithm OpenTLD works. Therefore, all the rights of the people I mentioned are completely reserved.

***********************************************/

int main(int argc, char** argv)
{


    srand(time(NULL));

    if( init() == 1 ) return 1;
    atexit(deinit);
    int key = -1;
    while( key == 'r' || key != 27 )
    {
        system("cls");
        if( acquireFrame() ){
            return 1;
        }
        process();

        draw();

        key = waitKey(1);

        if( key == 'r' ) reset();
    }


    return 0;
}



