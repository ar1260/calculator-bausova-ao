import subprocess
import pytest
import os

def test_calc1():
    res=subprocess.run(["build/app.exe", "--float"], input="1000000000 + 1000000000",text=True,capture_output=True)
    assert res.returncode == 0
    assert (2000000000.0000)
    
def test_calc2():
    res=subprocess.run(["build/app.exe"], input="1000000000 + 1000000000",text=True,capture_output=True)
    assert res.returncode == 0
    assert (2000000000)
    
def test_calc3():
    res=subprocess.run(["build/app.exe"], input="   43 -1 *       42  ",text=True,capture_output=True)
    assert res.returncode == 0
    assert (1)

def test_calc4():
    res=subprocess.run(["build/app.exe", "--float"], input="(2 + 2) * 2",text=True,capture_output=True)
    assert res.returncode == 0
    assert (8.0000)
    
def test_calc5():
    res=subprocess.run(["build/app.exe"], input="2 + 2 * 2",text=True,capture_output=True)
    assert res.returncode == 0
    assert (6.0000)
    
def test_calc6():
    res=subprocess.run(["build/app.exe", "--float"], input= "(((((((((((((((((((((((((((((4-1*((((((((((((((((((((((((((((((((((((((((2+2)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))",text=True,capture_output=True)
    assert res.returncode == 0
    expected_output = "0.0000" 
    assert res.stdout.strip() == expected_output
    
def test_calc7():
    res=subprocess.run(["build/app.exe"], input= "45 - 2",text=True,capture_output=True)
    assert res.returncode == 0
    assert (43)
    
def test_calc8():
    res=subprocess.run(["build/app.exe"], input= "2 - 3 * 6",text=True,capture_output=True)
    assert res.returncode == 0
    assert (-16)
    
def test_calc9():
    res=subprocess.run(["build/app.exe", "--float"], input= "2 - 3 * 6",text=True,capture_output=True)
    assert res.returncode == 0
    assert (-16.0000)
    
def test_calc10():
    res=subprocess.run(["build/app.exe"], input= "(4 - 2) / 3",text=True,capture_output=True)
    assert res.returncode == 0
    expected_output = "0" 
    assert res.stdout.strip() == expected_output
    
def test_calc11():
    res=subprocess.run(["build/app.exe", "--float"], input= "(4 - 2) / 3",text=True,capture_output=True)
    assert res.returncode == 0
    assert (0.6667)
    
def test_calc12():
    res=subprocess.run(["build/app.exe", "--float"], input= "158 / 9",text=True,capture_output=True)
    assert res.returncode == 0
    assert (17.5556)
    
def test_calc11():
    res=subprocess.run(["build/app.exe"], input= "(0 - 3) * (0 - 5)",text=True,capture_output=True)
    assert res.returncode == 0
    assert (15)
    
def test_err1():
    res=subprocess.run(["build/app.exe"], input= "int",text=True,capture_output=True)
    assert res.returncode != 0
    
def test_err2():
    res=subprocess.run(["build/app.exe", "--float"], input= "i + 1",text=True,capture_output=True)
    assert res.returncode != 0

def test_err3():
    res=subprocess.run(["build/app.exe", "--float"], input= "16 / 0",text=True,capture_output=True)
    assert res.returncode != 0
    
def test_err4():
    res=subprocess.run(["build/app.exe"], input= "16 / 0",text=True,capture_output=True)
    assert res.returncode != 0
    
def test_err5():
    res=subprocess.run(["build/app.exe", "--float"], input= "(10 + 5)8",text=True,capture_output=True)
    assert res.returncode != 0

def test_err6():
    res=subprocess.run(["build/app.exe", "--float"], input= "2 + 2 ) * 2",text=True,capture_output=True)
    assert res.returncode != 0
    
def test_err7():
    res=subprocess.run(["build/app.exe"], input= "(2 + 2  * 2",text=True,capture_output=True)
    assert res.returncode != 0
    
def test_err8():
    res=subprocess.run(["build/app.exe", "--float"], input= "1000000001 + 1000000000",text=True,capture_output=True)
    assert res.returncode != 0

def test_err9():
    res=subprocess.run(["build/app.exe", "--float"], input= "1000000 * 1000000",text=True,capture_output=True)
    assert res.returncode != 0
    



