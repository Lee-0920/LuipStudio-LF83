config.hardwareConfig =
{
    thermostatParam =
    {
        react =
        {
            proportion =
            {
                set = 90,
                real = 90,
            },
            integration =
            {
                set = 0.71,
                real = 0.71,
            },
            differential =
            {
                set = 80,
                real = 80,
            },
        },
        fungoid =
        {
            proportion =
            {
                set = 90,
                real = 90,
            },
            integration =
            {
                set = 0.71,
                real = 0.71,
            },
            differential =
            {
                set = 80,
                real = 80,
            },
        },
    },
    pumpFactor =
    {
        0.0005,
    },
    motorParam =
    {
        motorX =
        {
            speed = 1500,
            acceleration = 1500,
        },
        motorZ =
        {
            speed = 600,
            acceleration = 600,
        },
    },
    tempCalibrate =
    {
        react =
        {
            negativeInput = 1.9925,
            referenceVoltage = 2.5001,
            calibrationVoltage = 0,
        },
        fungoid =
        {
            negativeInput = 1.9925,
            referenceVoltage = 2.5001,
            calibrationVoltage = 0,
        },
    },
    pmtVol =
    {
        reference =
        {
            set = 511,
            real = 511,
        },
        measure =
        {
            set = 511,
            real = 511,
        },
    }
}
