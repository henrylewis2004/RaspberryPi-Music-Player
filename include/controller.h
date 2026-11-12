#ifndef CONTROLLER_H
#define CONTROLLER_H

#ifndef INPUT_COOLDOWN_US
#define INPUT_COOLDOWN_US 50*1000
#endif 


void controller_system_init(void);
void controller_system_loop(void);

#endif // !DEBUG
