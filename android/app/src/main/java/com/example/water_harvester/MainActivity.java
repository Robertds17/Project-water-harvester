package com.example.water_harvester;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.res.ResourcesCompat;

import android.graphics.Color;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import com.example.water_harvester.Classes.*;

import java.util.ArrayList;
import java.util.List;

import lecho.lib.hellocharts.model.PieChartData;
import lecho.lib.hellocharts.model.SliceValue;
import lecho.lib.hellocharts.view.PieChartView;

import static com.example.water_harvester.Classes.GraphBuilder.BuildAmbientHumPieChartData;
import static com.example.water_harvester.Classes.GraphBuilder.BuildAmbientTempPieChartData;
import static com.example.water_harvester.Classes.GraphBuilder.BuildFanRPMPieChartData;
import static com.example.water_harvester.Classes.GraphBuilder.BuildInsideHumPieChartData;
import static com.example.water_harvester.Classes.GraphBuilder.BuildInsideTempPieChartData;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // test stuff here


        // stop testing stuff here


        final FirebaseDatabase database = FirebaseDatabase.getInstance();

        final DatabaseReference dbRef = database.getReference();
        final DatabaseReference ambientTempRef = dbRef.child("ambientTemp");
        final DatabaseReference fanRPMRef = dbRef.child("fanRPM");
        final DatabaseReference insideHUMRef = dbRef.child("insideHum");
        final DatabaseReference insideTempRef = dbRef.child("insideTemp");
        final DatabaseReference ambientHumRef = dbRef.child("ambientHum");
        final DatabaseReference onOffRef = dbRef.child("onOff");

        final TextView seekbarRPMView = findViewById(R.id.rpmspeed);
        final Switch onOffSwitchView = findViewById(R.id.switch1);
        final Button fanChangeView = findViewById(R.id.fanChange);
        final SeekBar seekbarView = findViewById(R.id.seekBar);
        final EditText editFanRPMView = findViewById(R.id.editRPM);
        final PieChartView ambientTempView = findViewById(R.id.ambientTempView);
        final PieChartView fanRPMView = findViewById(R.id.fanRPMView);
        final PieChartView insideHUMView = findViewById(R.id.InsideHumView);
        final PieChartView insideTempView = findViewById(R.id.InsideTempView);
        final PieChartView ambientHumView = findViewById(R.id.ambientHumView);


        dbRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                DataStructure value = dataSnapshot.getValue(DataStructure.class);
                Log.d("Value", "Value: " + value);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w("Value", "Failed to read value with error: ", databaseError.toException());
            }
        });


        onOffSwitchView.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                Log.v("Switch State", "" + isChecked);
                onOffRef.setValue(isChecked);
            }
        });

        fanChangeView.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                int fanRpmConvert;
                try{
                    fanRpmConvert =  Integer.parseInt(editFanRPMView.getText().toString());
                } catch (NumberFormatException ex){
                    fanRpmConvert = 0;
                }
                Log.v("changeRPM", "" + fanRpmConvert);
                fanRPMRef.setValue(fanRpmConvert);
            }
        });


        seekbarView.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean b) {
                seekbarRPMView.setText(String.valueOf(progress));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                int seekbarRpmConvert = seekBar.getProgress();
                Log.v("RPM slider:", String.valueOf(seekbarRpmConvert));
                fanRPMRef.setValue(seekbarRpmConvert);
            }
        });

        ambientTempRef.addValueEventListener(new ValueEventListener() {
             @Override
             public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                 long value = (long)dataSnapshot.getValue();
                 Log.d("ambientTemp","" + value);
                 ambientTempView.setPieChartData(BuildAmbientTempPieChartData(value, getColorFromResource(R.color.lightOrange)));
             }

             @Override
             public void onCancelled(@NonNull DatabaseError databaseError) {
                 Log.w("ambientTemp", "Failed to read value with error: ", databaseError.toException());
             }
         });

        fanRPMRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                long value = (long)dataSnapshot.getValue();
                Log.d("fanRPMRef","fanRPMRef: " + value);
                fanRPMView.setPieChartData(BuildFanRPMPieChartData(value, getColorFromResource(R.color.lightYellow)));
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w("fanRPMRef", "Failed to read value with error: ", databaseError.toException());
            }
        });

        insideHUMRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                long value = (long)dataSnapshot.getValue();
                Log.d("insideHUMRef","insideHUMRef: " + value);
                insideHUMView.setPieChartData(BuildInsideHumPieChartData(value, getColorFromResource(R.color.lightBlue)));
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w("insideHUMRef", "Failed to read value with error: ", databaseError.toException());
            }
        });

        insideTempRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                long value = (long)dataSnapshot.getValue();
                Log.d("insideTempRef","insideTempRef: " + value);
                insideTempView.setPieChartData(BuildInsideTempPieChartData(value, getColorFromResource(R.color.lightOrange)));
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w("insideTempRef", "Failed to read value with error: ", databaseError.toException());
            }
        });

        ambientHumRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                long value = (long)dataSnapshot.getValue();
                Log.d("ambientHumRef","ambientHumRef: " + value);
                ambientHumView.setPieChartData(BuildAmbientHumPieChartData(value, getColorFromResource(R.color.lightBlue)));
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w("ambientHumRef", "Failed to read value with error: ", databaseError.toException());
            }
        });
    }

    private int getColorFromResource(int id){
        return ResourcesCompat.getColor(getResources(), id, null);
    }
}
