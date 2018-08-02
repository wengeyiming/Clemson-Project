using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ControlManager : MonoBehaviour {
    public GameObject cube;
    public GameObject joint1;
    public GameObject joint2;
    public GameObject joint3;
    public GameObject joint4;
    public GameObject joint5;
    public Text globalPosition;
    public Text globalRotationInEuler;
    public Text globalRotationInQuaternion;
    public Text transformationMatrix;

    private GameObject nowJoint;
    private int nowJointPosition;
    private float rotateVelocity = 1.0f;

    private Vector3 cubePosition;
    private Vector3[] translation = new Vector3[4];
    private Vector3[] eulerAngle = new Vector3[4];
    private Vector3 scale = new Vector3(1f, 1f, 1f);
    private Quaternion quaternion;
    // Use this for initialization
    void Start () {
        translation[0] = new Vector3(0, joint2.transform.position.y - joint1.transform.position.y, 0);
        translation[1] = new Vector3(0, joint3.transform.position.y - joint2.transform.position.y, 0);
        translation[2] = new Vector3(0, joint4.transform.position.y - joint3.transform.position.y, 0);
        translation[3] = new Vector3(0, joint5.transform.position.y - joint4.transform.position.y, 0);
        cubePosition = joint1.transform.position;
        cube.transform.position = cubePosition;
        nowJoint = joint1;
    }
	
    void calculateCubePosition()
    {
        Vector3 initialPosition = joint1.transform.position;
        Vector3 tempPosition = initialPosition;
        eulerAngle[0] = joint1.transform.localEulerAngles;
        eulerAngle[1] = joint2.transform.localEulerAngles;
        eulerAngle[2] = joint3.transform.localEulerAngles;
        eulerAngle[3] = joint4.transform.localEulerAngles;

        Matrix4x4 tMatrix = Matrix4x4.identity;

        for (int i = nowJointPosition-2; i>=0; i--)
        {
            Quaternion r1 = Quaternion.Euler(eulerAngle[i]);
            Matrix4x4 r = Matrix4x4.TRS(initialPosition, r1, scale);
            Matrix4x4 t = Matrix4x4.Translate(translation[i]);
            tMatrix = tMatrix * r * t;
            tempPosition = r.MultiplyPoint3x4(t.MultiplyPoint3x4(tempPosition));
        }
        cube.transform.position = tempPosition;
        globalPosition.text = "Global Position of Cube: \n" + cube.transform.position;
        transformationMatrix.text = "TransformationMatrix:\n" + tMatrix.ToString();
        Vector3 globalRotationEulerAngles = calculateEulerAngle(tMatrix);
        Quaternion globalRotationQuaternion = Quaternion.Euler(globalRotationEulerAngles);

        /*
         * these two words can also get quaternions and eulerAngles from the transformation Matrix.
        Quaternion globalRotationQuaternion = GetRotation(tMatrix);
        Vector3 globalRotationEulerAngles = globalRotationQuaternion.eulerAngles;
        */
        cube.transform.rotation = globalRotationQuaternion;
        globalRotationInEuler.text = "Global Rotation in\n EulerAngles:\n" + globalRotationEulerAngles;
        globalRotationInQuaternion.text = "Global Rotation in\n Quaternion:\n" + globalRotationQuaternion;    
    }

     Quaternion GetRotation(Matrix4x4 matrix)
     {
        return Quaternion.LookRotation(matrix.GetColumn(2), matrix.GetColumn(1));
     }

    Vector3 calculateEulerAngle(Matrix4x4 m)
    {
        if (m.m12 < +1)
        {
            if (m.m12 > -1)
            {
                float x = (Mathf.Sin(-m.m12)) * Mathf.Rad2Deg;
                float y = (Mathf.Atan2(m.m02, m.m22)) * Mathf.Rad2Deg;
                float z = (Mathf.Atan2(m.m10, m.m11)) * Mathf.Rad2Deg;
                return new Vector3(x, y, z);
            }
            else
            {
                float x = 90.0f;
                float y = (-Mathf.Atan2(-m.m01, m.m00)) * Mathf.Rad2Deg;
                float z = 0.0f;
                return new Vector3(x, y, z);
            }
        }
        else
        {
            float x = -90.0f;
            float y = (Mathf.Atan2(-m.m01, m.m00)) * Mathf.Rad2Deg;
            float z = 0.0f;
            return new Vector3(x, y, z);
        }
    }
    // Update is called once per frame
    void Update () {
		if(Input.GetKeyDown("1"))
        {
            nowJoint = joint1;
            nowJointPosition = 1;
            calculateCubePosition();
        }

        if (Input.GetKeyDown("2"))
        {
            nowJoint = joint2;
            nowJointPosition = 2;
            calculateCubePosition();
        }

        if (Input.GetKeyDown("3"))
        {
            nowJoint = joint3;
            nowJointPosition = 3;
            calculateCubePosition();
        }

        if (Input.GetKeyDown("4"))
        {
            nowJoint = joint4;
            nowJointPosition = 4;
            calculateCubePosition();
        }

        if (Input.GetKeyDown("5"))
        {
            nowJoint = joint5;
            nowJointPosition = 5;
            calculateCubePosition();
        }

        if (Input.GetKey("w"))
        {
            nowJoint.transform.Rotate(Vector3.right * rotateVelocity);
        }

        if (Input.GetKey("s"))
        {
            nowJoint.transform.Rotate(Vector3.left * rotateVelocity);
        }

        if (Input.GetKey("a"))
        {
            nowJoint.transform.Rotate(Vector3.forward * rotateVelocity);
        }

        if (Input.GetKey("d"))
        {
            nowJoint.transform.Rotate(Vector3.back * rotateVelocity);
        }

        if (Input.GetKey("q"))
        {
            nowJoint.transform.Rotate(Vector3.up * rotateVelocity);
        }

        if (Input.GetKey("e"))
        {
            nowJoint.transform.Rotate(Vector3.down * rotateVelocity);
        }
    }

}
