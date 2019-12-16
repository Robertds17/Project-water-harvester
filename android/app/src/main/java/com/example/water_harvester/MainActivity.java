package com.example.water_harvester;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.util.Log;
import android.widget.CompoundButton;
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
        System.out.println("!!MYLOG!! " + database);

        final DatabaseReference dbRef = database.getReference();

        System.out.println("!!MYLOG!! " + dbRef);

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

        Switch onOffSwitch = findViewById(R.id.switch1);

        onOffSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {

                if(isChecked){
                    Log.v("Switch State=", ""+isChecked);
                    dbRef.child("onOff").setValue(isChecked);
                } else if(!isChecked){
                    Log.v("Switch State=", ""+isChecked);
                    dbRef.child("onOff").setValue(isChecked);
                }
            }

        });

        DatabaseReference ambientTempRef = dbRef.child("ambientTemp");

        ambientTempRef.addValueEventListener(new ValueEventListener() {
             @Override
             public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                 int value = dataSnapshot.getValue(int.class);
                 Log.d("ambientTemp","ambientTemp: " + value);
                 PieChartView pieChartView = findViewById(R.id.ambientTempView);
                 pieChartView.setPieChartData(BuildAmbientTempPieChartData(value));
             }

             @Override
             public void onCancelled(@NonNull DatabaseError databaseError) {
                 Log.w("ambientTemp", "Failed to read value with error: ", databaseError.toException());
             }
         });

        DatabaseReference fanRPMRef = dbRef.child("fanRPM");

        fanRPMRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                int value = dataSnapshot.getValue(int.class);
                Log.d("fanRPMRef","fanRPMRef: " + value);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w("fanRPMRef", "Failed to read value with error: ", databaseError.toException());
            }
        });

        DatabaseReference insideHUMRef = dbRef.child("insideHum");

        insideHUMRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                int value = dataSnapshot.getValue(int.class);
                Log.d("insideHUMRef","insideHUMRef: " + value);
                PieChartView pieChartView = findViewById(R.id.InsideHumView);
                pieChartView.setPieChartData(BuildInsideHumPieChartData(value));
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w("insideHUMRef", "Failed to read value with error: ", databaseError.toException());
            }
        });

        DatabaseReference insideTempRef = dbRef.child("insideTemp");

        insideTempRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                int value = dataSnapshot.getValue(int.class);
                Log.d("insideTempRef","insideTempRef: " + value);
                PieChartView pieChartView = findViewById(R.id.InsideTempView);
                pieChartView.setPieChartData(BuildInsideTempPieChartData(value));
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w("insideTempRef", "Failed to read value with error: ", databaseError.toException());
            }
        });

        DatabaseReference ambientHumRef = dbRef.child("ambientHum");

        ambientHumRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                int value = dataSnapshot.getValue(int.class);
                Log.d("ambientHumRef","ambientHumRef: " + value);
                PieChartView pieChartView = findViewById(R.id.ambientHumView);
                pieChartView.setPieChartData(BuildAmbientHumPieChartData(value));
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w("ambientHumRef", "Failed to read value with error: ", databaseError.toException());
            }
        });
    }
}
