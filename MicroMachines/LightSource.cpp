#include <stdio.h>
#include <stdlib.h>

#include "LightSource.h"

LightSource::LightSource() {
	light = (Light*)malloc(sizeof(*light));
}

LightSource::LightSource(int lID) {
	light = (Light*)malloc(sizeof(*light));
	lightID = lID;
}
