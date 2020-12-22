package com.mysterycode.myjni;

/**
 * @author DeepInIt
 * @date 12/17/20
 * @description
 */
public class Person {
    private String name;
    private int age;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

//    public Person(){
//
//    }
//
//    public Person(String name) {
//        this.name = name;
//    }

    @Override
    public String toString() {
        return "Person{" +
                "name='" + name + '\'' +
                ", age=" + age +
                '}';
    }
}
