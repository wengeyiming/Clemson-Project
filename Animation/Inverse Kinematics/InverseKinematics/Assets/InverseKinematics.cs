using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using MatrixClass;

public class InverseKinematics : MonoBehaviour {

    public GameObject target;
    public GameObject joint1;
    public GameObject joint2;
    public GameObject joint3;

    private float maxLength = 0f;
    private float rotateVelocity = 100f;
    private float translateVelocity = 0.1f;
    private Vector3 targetPosition;
    // Use this for initialization
    void Start () {
        maxLength = joint3.transform.position.y - joint1.transform.position.y;
        targetPosition = joint3.transform.position;
    }
	
	// Update is called once per frame
	void Update () {
        target.transform.Translate(Input.GetAxis("Horizontal") * translateVelocity, Input.GetAxis("Vertical") * translateVelocity, 0);
        if (Input.GetKey("i"))
        {
            targetPosition = target.transform.position;
            if(targetPosition.magnitude > maxLength)
            {
                //shrink the target position to make it equal with maxlength to avoid shaking.
                targetPosition *= maxLength / targetPosition.magnitude;
            }
        }
        //set the precision to 0.1, which means the distance between target postion and joint3 position. The too high precision will cause a lot of calculation.
        if (Vector3.Distance(targetPosition, joint3.transform.position) > 0.1f )
        {
            _Matrix angleMatrix = calAngels(targetPosition);
            joint1.transform.Rotate(0f, 0f, angleMatrix.read(0, 0) * Time.deltaTime * rotateVelocity);
            joint2.transform.Rotate(0f, 0f, angleMatrix.read(1, 0) * Time.deltaTime * rotateVelocity);
        }

    }

    _Matrix calAngels(Vector3 G)
    {
        Vector3 E = joint3.transform.position - joint1.transform.position;
        Vector3 p = joint2.transform.position - joint1.transform.position;
        _Matrix V = new _Matrix(3, 1);
        V.write(0, 0, (G - E).x);
        V.write(1, 0, (G - E).y);
        V.write(2, 0, (G - E).z);
        _Matrix J = new _Matrix(3, 2);
        Vector3 zAxis = new Vector3(0, 0, 1);
        J.write(0, 0, Vector3.Cross(zAxis, E).x);
        J.write(1, 0, Vector3.Cross(zAxis, E).y);
        J.write(2, 0, Vector3.Cross(zAxis, E).z);
        J.write(0, 1, Vector3.Cross(zAxis, E - p).x);
        J.write(1, 1, Vector3.Cross(zAxis, E - p).y);
        J.write(2, 1, Vector3.Cross(zAxis, E - p).z);
        _Matrix transpositionMatrixJ = _Matrix_Calc.transpositionMatrix(J);
        _Matrix transpositionMatrixJJ = _Matrix_Calc.multiply(transpositionMatrixJ, J);
        _Matrix inversetranspositionMatrixJJ = _Matrix_Calc.Inverse(transpositionMatrixJJ);
        _Matrix inverseMatrix = _Matrix_Calc.multiply(inversetranspositionMatrixJJ , transpositionMatrixJ);
        _Matrix angleMatrix = _Matrix_Calc.multiply(inverseMatrix, V);
        return angleMatrix;
    }

}
