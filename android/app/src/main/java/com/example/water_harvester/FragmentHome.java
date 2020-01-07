package com.example.water_harvester;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import static com.example.water_harvester.MainActivity.getCFragmentManager;



public class FragmentHome extends Fragment{
    private View view;


    private Button controlPanelButton;
    private Button pointsButton;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_home, container, false);

        controlPanelButton = view.findViewById(R.id.controlpanelButton);
        pointsButton = view.findViewById(R.id.pointsButton);

        controlPanelButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getCFragmentManager().beginTransaction().replace(R.id.fragment_container, new FragmentFirebaseData()).commit();
            }
        });

        return view;
    }

}
