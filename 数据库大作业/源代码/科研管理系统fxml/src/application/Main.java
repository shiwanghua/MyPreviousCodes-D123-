package application;

import javafx.application.Application;
import javafx.stage.Stage;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class Main extends Application {
	@Override
	public void start(Stage primaryStage) {
		LoginPart(primaryStage);
	}

	private void LoginPart(Stage primaryStage)
	{
		try {
			
            Parent root = FXMLLoader.load(getClass().getResource("ui_login.fxml"));
            primaryStage.setTitle("���й���ϵͳ--��¼����");
            primaryStage.setScene(new Scene(root,330,260));
            primaryStage.show();
        
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	public static void main(String[] args) {
		 Application.launch(Main.class, args);
	}
}
