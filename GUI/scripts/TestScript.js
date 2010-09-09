print("Testing js script ...",time);


try
{
    print("NbModules", modules.length);
    print("Time", time);
    print("Period",period);
    for (j = 0; j < modules.length; j++)
    {
        print("Module : ",j);

        for (i = 0; i < modules[j].variables.length; i++)
        {
            print("Variable id : ",i);
            print("Name :",modules[j].variables[i].getName());
            //print("Type :",modules[j].variables[i].getType());
            //print("MemType :",modules[j].variables[i].getMemType());
            print("Offset :",modules[j].variables[i].getOffset());
            print("Description :",modules[j].variables[i].getDescription());
            print("Version :",modules[j].variables[i].getVersion());
            print("DeviceID :",modules[j].variables[i].getDeviceID());
            print("Value :",modules[j].variables[i].getValue());
            print("\n");
        }//for i
    }//for j


}
catch(e)
{
    print(e);
}


//freq = 0.1;
//amplitude = 100;
//step = time / 10;
//sin_value = amplitude * Math.sin(step * 2 * Math.PI * freq * period)  +200;

print("Testing js done!", time);
