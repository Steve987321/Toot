# Toot

A very small and minimal scripting language to use in your program. 

## Building 

Use the premake5 script to generate project files. 

Project layout: 
```bash
Compiler/	# (Application) Compiles string to instructions 
TVM/		# (Static lib)  VM that runs instructions
``` 

Currently the compiler application is used for testing and will not take any arguments. 

## Syntax Examples

```c
// Car controller script
// Use A and D to rotate the wheels
// Space is crouch?

// store a factor to use to set the distance for the wheel to body springs
once float spring_wheel_dist_factor = 1.0; 

once 
{
	// saves the spring properties so this can apply a stiffness factor 
	CESaveSpringStates();
}

int key_d = 3;
int key_a = 0;
int key_space = 57;

if (IsKeyDown(key_space) == 1)
{
	if (spring_wheel_dist_factor > 0.0)
	{
		// charge jump
		spring_wheel_dist_factor = spring_wheel_dist_factor - GetDT();  
	}
	else 
	{
		// clamp to 0 
		spring_wheel_dist_factor = 0.0; 
	}
}
else 
{
	// release jump
	spring_wheel_dist_factor = 1.0; 
}

float gas = 0.0;

if (IsKeyDown(key_d) == 1)
{
	gas = gas + 10.0;
}
if (IsKeyDown(key_a) == 1)
{
	gas = gas - 10.0;
}

CESetSpringDistanceFactor(spring_wheel_dist_factor);
CEApplyGas(gas);
```

## Usage Examples 

Currently being used for my 2d physics project to interact with objects. 
https://github.com/Steve987321/Fizzix/tree/master
