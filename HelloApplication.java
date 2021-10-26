package com.example.calcolatrice;

import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.FlowPane;
import javafx.stage.Stage;

import java.io.IOException;

public class HelloApplication extends Application {
    int i,j;
    @Override
    public void start(Stage stage) throws IOException {
        stage.setTitle("Calcolatrice");
        FlowPane root=new FlowPane();
        root.setPadding(new Insets(5, 0, 5, 0));
        root.setVgap(5);
        root.setHgap(5);
        root.setPrefWrapLength(170);
        root.setStyle("-fx-background-color: DAE6F3;");
        Label label1=new Label();
        Label label2=new Label();
        Label label3=new Label();
        Button bu=new Button("=");
        Button bsom=new Button("+");
        Button bsot=new Button("-");
        Button bmol=new Button("x");
        Button bdiv=new Button(":");
        Button bc=new Button("C");
        Button b1=new Button("1");
        Button b2=new Button("2");
        Button b3=new Button("3");
        Button b4=new Button("4");
        Button b5=new Button("5");
        Button b6=new Button("6");
        Button b7=new Button("7");
        Button b8=new Button("8");
        Button b9=new Button("9");
        Button b0=new Button("0");
        bu.setOnAction(value ->   {
            j = Integer.valueOf(label1.getText());
            if((label2.getText()).equals("+")){
                i=i+j;
            } else if ((label2.getText()).equals("-")) {
                i = i - j;
            } else if ((label2.getText()).equals("*")){
                i=i*j;
            }else if ((label2.getText()).equals("/")){
                if(j!=0){
                    i = i / j;
                }
            }
            label3.setText(String.valueOf(i));
            label2.setText("");
            label1.setText("");
        });
        bdiv.setOnAction(value ->   {
            if((label1.getText()).isEmpty()){
                i=Integer.valueOf(label3.getText());
            }else {
                i = Integer.valueOf(label1.getText());
                label3.setText(label1.getText());
            }
            label2.setText("/");
            label1.setText("");
        });
        bmol.setOnAction(value ->   {
            if((label1.getText()).isEmpty()){
                i=Integer.valueOf(label3.getText());
            }else {
                i = Integer.valueOf(label1.getText());
                label3.setText(label1.getText());
            }
            label2.setText("*");
            label1.setText("");
        });
        bsot.setOnAction(value ->   {
            if((label1.getText()).isEmpty()){
                i=Integer.valueOf(label3.getText());
            }else {
                i = Integer.valueOf(label1.getText());
                label3.setText(label1.getText());
            }
            label2.setText("-");
            label1.setText("");
        });
        bsom.setOnAction(value ->  {
            if((label1.getText()).isEmpty()){
                i=Integer.valueOf(label3.getText());
            }else {
                i = Integer.valueOf(label1.getText());
                label3.setText(label1.getText());
            }
            label2.setText("+");
            label1.setText("");
        });
        bc.setOnAction(value -> {
            i=0;
            j=0;
            label1.setText("");
            label2.setText("");
            label3.setText("");
        });
        label1.setPrefSize(300, 45);
        label2.setPrefSize(300, 10);
        label3.setPrefSize(300, 45);
        b1.setPrefSize(67, 67);
        b2.setPrefSize(67, 67);
        b3.setPrefSize(67, 67);
        b4.setPrefSize(67, 67);
        b5.setPrefSize(67, 67);
        b6.setPrefSize(67, 67);
        b7.setPrefSize(67, 67);
        b8.setPrefSize(67, 67);
        b9.setPrefSize(67, 67);
        b0.setPrefSize(67, 67);
        bsom.setPrefSize(67, 67);
        bsot.setPrefSize(67, 67);
        bmol.setPrefSize(67, 67);
        bdiv.setPrefSize(67, 67);
        bu.setPrefSize(67, 67);
        bc.setPrefSize(67, 67);
        b1.setOnAction(value -> label1.setText(label1.getText()+"1"));
        b2.setOnAction(value -> label1.setText(label1.getText()+"2"));
        b3.setOnAction(value -> label1.setText(label1.getText()+"3"));
        b4.setOnAction(value -> label1.setText(label1.getText()+"4"));
        b5.setOnAction(value -> label1.setText(label1.getText()+"5"));
        b6.setOnAction(value -> label1.setText(label1.getText()+"6"));
        b7.setOnAction(value -> label1.setText(label1.getText()+"7"));
        b8.setOnAction(value -> label1.setText(label1.getText()+"8"));
        b9.setOnAction(value -> label1.setText(label1.getText()+"9"));
        b0.setOnAction(value -> label1.setText(label1.getText()+"0"));
        root.getChildren().add(label1);
        root.getChildren().add(label2);
        root.getChildren().add(label3);
        root.getChildren().add(b1);
        root.getChildren().add(b2);
        root.getChildren().add(b3);
        root.getChildren().add(bsom);
        root.getChildren().add(b4);
        root.getChildren().add(b5);
        root.getChildren().add(b6);
        root.getChildren().add(bsot);
        root.getChildren().add(b7);
        root.getChildren().add(b8);
        root.getChildren().add(b9);
        root.getChildren().add(bmol);
        root.getChildren().add(bu);
        root.getChildren().add(b0);
        root.getChildren().add(bc);
        root.getChildren().add(bdiv);
        Scene scene= new Scene(root);
        stage.setScene(scene);
        stage.setMaxWidth(300);
        stage.setMaxHeight(460);
        stage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}