#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum {
    SENSOR_TYPE_TEMP = 0,
    SENSOR_TYPE_PRESSURE,
    SENSOR_TYPE_HUMIDITY,
    SENSOR_TYPE_UNKNOWN
} SensorType;

typedef struct {
    unsigned int id;          // bit 31..16: type, bit 15..0: index
    SensorType type;
    char name[20];            // Null-terminated sensor name
    double last_value;
    unsigned long timestamp;
    unsigned int flags;       // Bit 0: active, Bit 1: error, Bit 2: calibrated
} SensorRecord;


static SensorRecord *sensors = NULL;
static size_t sensor_capacity = 0;


int init_database(size_t capacity);
void deinit_database(void);
unsigned int make_sensor_id(SensorType type, unsigned int index);
int register_sensor(SensorType type, const char *name, unsigned int *out_id);



int init_database(size_t capacity) {

    sensors = (SensorRecord *)malloc(capacity * sizeof(SensorRecord));
    if (sensors == NULL) {
        return -1;
    }

    sensor_capacity = capacity;

    for (size_t i = 0; i < capacity; i++) {
        sensors[i].id = 0;
        sensors[i].type = SENSOR_TYPE_UNKNOWN;
        sensors[i].name[0] = '\0';
        sensors[i].last_value = 0.0;
        sensors[i].timestamp = 0;
        sensors[i].flags = 0;   // unused
    }

    return 0;
}

void deinit_database(void) {

    if (sensors != NULL) {
        free(sensors);
        sensors = NULL;
    }

    sensor_capacity = 0;
}

unsigned int make_sensor_id(SensorType type, unsigned int index) {

    if (index >= sensor_capacity) {
        return 0;   // error
    }

    return ((unsigned int)type << 16) | (index & 0xFFFF);
}


int register_sensor(SensorType type, const char *name, unsigned int *out_id) {

    if (sensors == NULL || out_id == NULL || name == NULL) {
        return -1;
    }

    for (size_t i = 0; i < sensor_capacity; i++) {

        if (sensors[i].flags == 0) {

            sensors[i].id = make_sensor_id(type, (unsigned int)i);
            sensors[i].type = type;

            strncpy(sensors[i].name, name, sizeof(sensors[i].name) - 1);
            sensors[i].name[sizeof(sensors[i].name) - 1] = '\0';

            sensors[i].last_value = 0.0;
            sensors[i].timestamp = 0;

            sensors[i].flags = 0x1;

            *out_id = sensors[i].id;
            return 0;  
        }
    }

    return -1;  
}

int main() {

    unsigned int sensor_id;

    if (init_database(5) != 0) {
        printf("Memory allocation failed\n");
        return 1;
    }

    if (register_sensor(SENSOR_TYPE_TEMP, "TempSensor", &sensor_id) == 0) {
        printf("Sensor registered, ID = %u\n", sensor_id);
    } else {
        printf("Sensor registration failed\n");
    }

    deinit_database();
    return 0;
}
