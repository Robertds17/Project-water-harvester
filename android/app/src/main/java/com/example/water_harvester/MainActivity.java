package com.example.water_harvester;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        FirebaseDatabase database = FirebaseDatabase.getInstance();
        System.out.println("!!MYLOG!! " + database);

        DatabaseReference dbRef = database.getReference();

        System.out.println("!!MYLOG!! " + dbRef);


        dbRef.setValue("Hello, World!");

        dbRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                String value = dataSnapshot.getValue(String.class);
                Log.d("Value", "Value: " + value);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w("Value", "Failed to read value with error: ", databaseError.toException());
            }
        });
    }
}
