#ifndef CONTORLS_H_
#define CONTORLS_H_

struct Controls
{
   static bool leftMousePressed;
   static bool rightMousePressed;
   static bool windowResized;
   static int mouseX;
   static int mouseY;
   static void loop();
};



#endif // CONTORLS_H_
