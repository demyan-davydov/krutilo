#include <mbed.h>

#include "surface_ili9341.h"
#include "keypad.h"
#include "stepper_pwm.h"

#include "brush/brush_solid.h"
#include "brush/brush_strip.h"
//#include "brush/brush_linear_gradient.h"
//#include "brush/brush_radial_gradient.h"
//#include "pwmout_x.h"

#include <string>


#include "application/application.h"
#include "application/widget_app/main_wizard.h"

int main()
{
    //app().showSplash();

    //MainWizard root(nullptr, app().screenRect());
    Widget::createRoot<MainWizard>(app().screenRect());

    app().run();
}
