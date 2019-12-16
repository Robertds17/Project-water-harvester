package com.example.water_harvester.Classes;

import android.graphics.Color;

import com.example.water_harvester.R;

import java.util.ArrayList;
import java.util.List;

import lecho.lib.hellocharts.model.PieChartData;
import lecho.lib.hellocharts.model.SliceValue;
import lecho.lib.hellocharts.view.PieChartView;

public class GraphBuilder {
    public static PieChartData BuildPieChartData(int value, String label, String centerText1, String centerText2, int fontSize, int color){
        List<SliceValue> pieData = new ArrayList<>();
            pieData.add(new SliceValue(value).setColor(color).setLabel(label));
            pieData.add(new SliceValue(100-value).setLabel(""));

        return new PieChartData(pieData)
                .setHasLabels(true)
                .setHasCenterCircle(true)
                .setCenterText1(centerText1)
                .setCenterText1FontSize(fontSize)
                .setCenterText2(centerText2)
                .setCenterText2FontSize(fontSize);
    }

    public static PieChartData BuildAmbientTempPieChartData(int value){
        return BuildPieChartData(value * 2, value + "°C", "Ambient", "temperature", 10, Color.RED);
    }

    public static PieChartData BuildAmbientHumPieChartData(int value){
        return BuildPieChartData(value, value + "%", "Ambient", "humidity", 10, Color.BLUE);
    }
}
