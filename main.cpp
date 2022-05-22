#include "internal/pkg/app/app.h"

int main() {
    App *app = new App();
    app->loop();
    return 0;
}