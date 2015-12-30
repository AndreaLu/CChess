// Definition of control variables
#include "../Controls.h"
bool Controls::leftMousePressed = false;
bool Controls::rightMousePressed = false;
bool Controls::windowResized = false;
int  Controls::mouseX = 0;
int  Controls::mouseY = 0;

void Controls::loop()
{
   if( leftMousePressed )
      leftMousePressed = false;
   if( rightMousePressed )
      rightMousePressed = false;
   if( windowResized )
      windowResized = false;
}
