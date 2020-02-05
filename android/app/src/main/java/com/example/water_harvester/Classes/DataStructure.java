package com.example.water_harvester.Classes;

public class DataStructure {
    private long ambientHum = 0;
    private long ambientTemp = 0;
    private long insideTemp = 0;
    private long insideHum = 0;
    private long fanRPM = 0;
    private boolean onOff = true;

    public void setAmbientHum(int value){
        ambientHum = value;
    }

    public long getAmbientHum(){
        return ambientHum;
    }

    public void setAmbientTemp(int value){
        ambientTemp = value;
    }

    public long getAmbientTemp(){
        return ambientTemp;
    }

    public void setInsideTemp(int value){
        insideTemp = value;
    }

    public long getInsideTemp(){
        return insideTemp;
    }

    public void setInsideHum(int value){
        insideHum = value;
    }

    public long getInsideHum(){
        return insideHum;
    }

    public void setFanRPM(int value){
        fanRPM = value;
    }

    public long getFanRPM(){
        return fanRPM;
    }

    public boolean getOnOff() { return onOff; }
}
