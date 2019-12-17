package com.example.water_harvester.Classes;

import android.content.Context;
import android.graphics.Color;
import android.os.Build;

import androidx.core.content.res.ResourcesCompat;

import com.example.water_harvester.R;

import java.util.ArrayList;
import java.util.List;

import lecho.lib.hellocharts.model.PieChartData;
import lecho.lib.hellocharts.model.SliceValue;
import lecho.lib.hellocharts.view.PieChartView;

public class GraphBuilder {
    private static PieChartData BuildPieChartData(int value, String label, String centerText1, String centerText2, int fontSize, int color){
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

    public static PieChartData BuildAmbientTempPieChartData(int value, int color){
        return BuildPieChartData(value * 2, value + "°C", "Ambient", "temperature", 10, color);
    }

    public static PieChartData BuildAmbientHumPieChartData(int value, int color){
        return BuildPieChartData(value, value + "%", "Ambient", "humidity", 10, color);
    }

    public static PieChartData BuildInsideTempPieChartData(int value, int color){
        return BuildPieChartData(value * 2, value + "°C", "Inside", "temperature", 10, color);
    }

    public static PieChartData BuildInsideHumPieChartData(int value, int color){
        return BuildPieChartData(value, value + "%", "Inside", "humidity", 10, color);
    }

    public static PieChartData BuildFanRPMPieChartData(int value, int color){
        return BuildPieChartData(value, value * 25 + "rpm", "Fan", "RPM", 10, color);
    }
}
