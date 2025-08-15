local enemyGroups = {}
-- Enemy group is a array of enemy ids.. you can use nil if you don't want to spawn one in that location.
-- Location corresponds to the position on the map where they will spawn at.
enemyGroups.groups = {
    { 1, },
    { 1, nil, 2 }
}
return enemyGroups
