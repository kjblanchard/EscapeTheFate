using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CardMover : MonoBehaviour
{

    public bool isSelected = false;
    private Vector3 defaultLocation;
    // Start is called before the first frame update
    void Start()
    {
        defaultLocation = gameObject.transform.position;
    }

    // Update is called once per frame
    void Update()
    {
     if(isSelected == true)
        {
            gameObject.transform.position = new Vector3((defaultLocation.x+10),defaultLocation.y,defaultLocation.z);
        }
     if(isSelected == false)
        {
            gameObject.transform.position = defaultLocation;
        }
        
    }
}

