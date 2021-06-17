function setupFeatureMap ()
    AIShooter.addInterval ("opp_distance", 0, 0.0, 0.05);
    AIShooter.addInterval ("opp_distance", 1, 0.05, 0.1);
    AIShooter.addInterval ("opp_distance", 2, 0.1, 0.15);
    AIShooter.addInterval ("opp_distance", 3, 0.15, 0.2);
    AIShooter.addInterval ("opp_distance", 4, 0.2, 0.25);
    AIShooter.addInterval ("opp_distance", 5, 0.25, 0.3);
    AIShooter.addInterval ("opp_distance", 6, 0.3, 0.35);
    AIShooter.addInterval ("opp_distance", 7, 0.35, 0.4);
    AIShooter.addInterval ("opp_distance", 8, 0.4, 0.45);
    AIShooter.addInterval ("opp_distance", 9, 0.45, 0.5);
    AIShooter.addInterval ("opp_distance", 10, 0.5, 0.75);
    AIShooter.addInterval ("opp_distance", 11, 0.75, 1.0);
    AIShooter.addInterval ("opp_distance", 12, 1.0, 1.5);
    AIShooter.addInterval ("opp_distance", 13, 1.5, math.huge);
    
    AIShooter.addInterval ("opp_dir_to", 0, -math.huge, -175.0);
    AIShooter.addInterval ("opp_dir_to", 1, -175.0, -135.0);
    AIShooter.addInterval ("opp_dir_to", 2, -135.0, -90.0);
    AIShooter.addInterval ("opp_dir_to", 3, -90.0, -45.0);
    AIShooter.addInterval ("opp_dir_to", 4, -45.0, -15.0);
    AIShooter.addInterval ("opp_dir_to", 5, -15.0, -5.0);
    AIShooter.addInterval ("opp_dir_to", 6, -5.0, 0.0);
    AIShooter.addInterval ("opp_dir_to", 7, 0.0, 5.0);
    AIShooter.addInterval ("opp_dir_to", 8, 5.0, 15.0);
    AIShooter.addInterval ("opp_dir_to", 9, 15.0, 45.0);
    AIShooter.addInterval ("opp_dir_to", 10, 45.0, 90.0);
    AIShooter.addInterval ("opp_dir_to", 11, 90.0, 135.0);
    AIShooter.addInterval ("opp_dir_to", 12, 135.0, 175.0);
    AIShooter.addInterval ("opp_dir_to", 13, 175.0, math.huge);

    AIShooter.addInterval ("opp_dir_from", 0, -math.huge, -175.0);
    AIShooter.addInterval ("opp_dir_from", 1, -175.0, -135.0);
    AIShooter.addInterval ("opp_dir_from", 2, -135.0, -90.0);
    AIShooter.addInterval ("opp_dir_from", 3, -90.0, -45.0);
    AIShooter.addInterval ("opp_dir_from", 4, -45.0, -15.0);
    AIShooter.addInterval ("opp_dir_from", 5, -15.0, -5.0);
    AIShooter.addInterval ("opp_dir_from", 6, -5.0, 0.0);
    AIShooter.addInterval ("opp_dir_from", 7, 0.0, 5.0);
    AIShooter.addInterval ("opp_dir_from", 8, 5.0, 15.0);
    AIShooter.addInterval ("opp_dir_from", 9, 15.0, 45.0);
    AIShooter.addInterval ("opp_dir_from", 10, 45.0, 90.0);
    AIShooter.addInterval ("opp_dir_from", 11, 90.0, 135.0);
    AIShooter.addInterval ("opp_dir_from", 12, 135.0, 175.0);
    AIShooter.addInterval ("opp_dir_from", 13, 175.0, math.huge);
    
    AIShooter.addInterval ("opp_vnorm", 0, 0.0, 0.001);
    AIShooter.addInterval ("opp_vnorm", 1, 0.001, 0.0015);
    AIShooter.addInterval ("opp_vnorm", 2, 0.0015, 0.002);
    AIShooter.addInterval ("opp_vnorm", 3, 0.002, 0.0025);
    AIShooter.addInterval ("opp_vnorm", 4, 0.0025, 0.003);
    AIShooter.addInterval ("opp_vnorm", 5, 0.003, 0.0035);
    AIShooter.addInterval ("opp_vnorm", 6, 0.0035, 0.004);
    AIShooter.addInterval ("opp_vnorm", 7, 0.004, 0.0045);
    AIShooter.addInterval ("opp_vnorm", 8, 0.0045, 0.005);
    AIShooter.addInterval ("opp_vnorm", 9, 0.005, math.huge);

	AIShooter.addInterval ("opp_ddir_to", 0, -math.huge, -175.0);
    AIShooter.addInterval ("opp_ddir_to", 1, -175.0, -135.0);
    AIShooter.addInterval ("opp_ddir_to", 2, -135.0, -90.0);
    AIShooter.addInterval ("opp_ddir_to", 3, -90.0, -45.0);
    AIShooter.addInterval ("opp_ddir_to", 4, -45.0, -15.0);
    AIShooter.addInterval ("opp_ddir_to", 5, -15.0, -5.0);
    AIShooter.addInterval ("opp_ddir_to", 6, -5.0, 0.0);
    AIShooter.addInterval ("opp_ddir_to", 7, 0.0, 5.0);
    AIShooter.addInterval ("opp_ddir_to", 8, 5.0, 15.0);
    AIShooter.addInterval ("opp_ddir_to", 9, 15.0, 45.0);
    AIShooter.addInterval ("opp_ddir_to", 10, 45.0, 90.0);
    AIShooter.addInterval ("opp_ddir_to", 11, 90.0, 135.0);
    AIShooter.addInterval ("opp_ddir_to", 12, 135.0, 175.0);
    AIShooter.addInterval ("opp_ddir_to", 13, 175.0, math.huge);

    AIShooter.addInterval ("opp_ddir_from", 0, -math.huge, -175.0);
    AIShooter.addInterval ("opp_ddir_from", 1, -175.0, -135.0);
    AIShooter.addInterval ("opp_ddir_from", 2, -135.0, -90.0);
    AIShooter.addInterval ("opp_ddir_from", 3, -90.0, -45.0);
    AIShooter.addInterval ("opp_ddir_from", 4, -45.0, -15.0);
    AIShooter.addInterval ("opp_ddir_from", 5, -15.0, -5.0);
    AIShooter.addInterval ("opp_ddir_from", 6, -5.0, 0.0);
    AIShooter.addInterval ("opp_ddir_from", 7, 0.0, 5.0);
    AIShooter.addInterval ("opp_ddir_from", 8, 5.0, 15.0);
    AIShooter.addInterval ("opp_ddir_from", 9, 15.0, 45.0);
    AIShooter.addInterval ("opp_ddir_from", 10, 45.0, 90.0);
    AIShooter.addInterval ("opp_ddir_from", 11, 90.0, 135.0);
    AIShooter.addInterval ("opp_ddir_from", 12, 135.0, 175.0);
    AIShooter.addInterval ("opp_ddir_from", 13, 175.0, math.huge);

	AIShooter.addInterval ("opp_hp", 0, -math.huge, 5);
    AIShooter.addInterval ("opp_hp", 1, 5, 10);
    AIShooter.addInterval ("opp_hp", 2, 10, 20);
    AIShooter.addInterval ("opp_hp", 3, 20, 40);
    AIShooter.addInterval ("opp_hp", 4, 40, 80);
    AIShooter.addInterval ("opp_hp", 5, 80, math.huge);

    AIShooter.addInterval ("my_hp", 0, -math.huge, 5);
    AIShooter.addInterval ("my_hp", 1, 5, 10);
    AIShooter.addInterval ("my_hp", 2, 10, 20);
    AIShooter.addInterval ("my_hp", 3, 20, 40);
    AIShooter.addInterval ("my_hp", 4, 40, 80);
    AIShooter.addInterval ("my_hp", 5, 80, math.huge);

end

function turnTree ()
end

function accelTree ()
end

function fireTree ()
end

