package de.saba;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.time.LocalDateTime;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicInteger;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;

import de.saba.model.Clock;
import de.saba.model.ClockData;
import de.saba.model.ClockDial;
import de.saba.model.ClockFace;
import de.saba.model.ClockPainter;
import de.saba.model.ClockPainter.Ordering;
import de.saba.model.ClockStore;
import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.ButtonType;
import javafx.scene.control.CheckMenuItem;
import javafx.scene.control.ComboBox;
import javafx.scene.control.ContextMenu;
import javafx.scene.control.Label;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.control.TextInputDialog;
import javafx.scene.control.ToolBar;
import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeView;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyCodeCombination;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.FileChooser;
import javafx.stage.FileChooser.ExtensionFilter;
import javafx.stage.Stage;
import javafx.util.Duration;

public class Main extends Application implements Constants
{
  private Stage mainStage;

  private File initialDirectory;
  private File dataFile;

  private MenuItem saveMenu;
  private MenuItem saveAsMenu;
  private MenuItem exportMenu;

  private Menu orderMenu;

  private ObservableList<String> animations = FXCollections.observableArrayList( "Stop", "Now", " * 2", " * 5", " * 10",
      " * 25", " * 100" );
  private Timeline timeline;
  private LocalDateTime time;

  private ClockPane clockPane;

  private TreeView<ClockData> tree;
  private TreeItem<ClockData> root;

  private Clock clock = new Clock( "Zeiger" );

  protected ClockPainter selectedClockPainter;

  @Override
  public void start(Stage primaryStage)
  {
    mainStage = primaryStage;

    // ClockFace clockFace = new ClockFace( "Test" );
    // clock.getChildren().add( clockFace );

    try
    {
      MenuBar menuBar = new MenuBar();
      initializeMenu( menuBar );

      HBox hbox = new HBox( 4 );

      root = setupTree( clock );
      tree = new TreeView<>( root );
      tree.setShowRoot( false );

      MenuItem addFace = new MenuItem( "New Clock face" );
      addFace.setOnAction( new EventHandler<ActionEvent>()
      {
        @Override
        public void handle(ActionEvent event)
        {
          TextInputDialog newFace = new TextInputDialog();
          newFace.setTitle( "New Clock face" );
          newFace.setHeaderText( "Enter the Face name" );
          newFace.setContentText( "Name:" );
          Optional<String> result = newFace.showAndWait();

          result.ifPresent( name ->
          {
            ClockFace clockFace = new ClockFace( name );
            clock.getClockFaces().getChildren().add( clockFace );
            root.getChildren().get( 0 ).getChildren().add( setupTree( clockFace ) );
          } );

        }
      } );

      MenuItem addDial = new MenuItem( "New Clock dial" );
      addDial.setOnAction( new EventHandler<ActionEvent>()
      {
        @Override
        public void handle(ActionEvent event)
        {
          TextInputDialog newFace = new TextInputDialog();
          newFace.setTitle( "New Clock dial" );
          newFace.setHeaderText( "Enter the Dial name" );
          newFace.setContentText( "Name:" );
          Optional<String> result = newFace.showAndWait();

          result.ifPresent( name ->
          {
            ClockDial clockDial = new ClockDial( name );
            clock.getClockDials().getChildren().add( clockDial );
            root.getChildren().get( 1 ).getChildren().add( setupTree( clockDial ) );
          } );

        }
      } );

      orderMenu = new Menu( "Order" );
      orderMenu.setDisable( true );
      for (ClockPainter.Ordering ordering : ClockPainter.Ordering.values())
      {
        CheckMenuItem item = new CheckMenuItem( ordering.toString() );
        orderMenu.getItems().add( item );

        item.setOnAction( ah ->
        {
          if (selectedClockPainter != null)
          {
            selectedClockPainter.setOrdering( ordering );
            clockPane.repaint( time );
          }
          setupOrderMenu();
        } );
      }

      ContextMenu contextMenu = new ContextMenu( addFace, addDial, orderMenu );
      tree.setContextMenu( contextMenu );

      tree.getSelectionModel().selectedItemProperty().addListener( new ChangeListener<TreeItem<ClockData>>()
      {
        @Override
        public void changed(ObservableValue<? extends TreeItem<ClockData>> observable, TreeItem<ClockData> oldValue,
            TreeItem<ClockData> newValue)
        {
          if (newValue != null && newValue.getValue() instanceof ClockStore)
            clockPane.setClockPainter( ((ClockStore) newValue.getValue()) );

          if (newValue != null && newValue.getValue() instanceof ClockPainter)
            selectedClockPainter = (ClockPainter) newValue.getValue();
          else
            selectedClockPainter = null;

          setupOrderMenu();
        }
      } );
      clockPane = new ClockPane();

      hbox.getChildren().addAll( tree, clockPane );

      ToolBar toolbar = createToolbar();

      VBox vbox = new VBox( 4 );
      vbox.getChildren().addAll( menuBar, toolbar, hbox );

      Scene scene = new Scene( vbox, 150 + 2 * (int) offsetX, 70 + 2 * (int) offsetY );
      scene.getStylesheets().add( getClass().getResource( "application.css" ).toExternalForm() );

      primaryStage.setScene( scene );
      primaryStage.show();
    }
    catch (Exception e)
    {
      e.printStackTrace();
    }
  }

  private void setupOrderMenu()
  {
    orderMenu.setDisable( selectedClockPainter == null );
    if (selectedClockPainter != null)
    {
      Ordering selected = selectedClockPainter.getOrdering();
      orderMenu.getItems()
          .forEach( om -> ((CheckMenuItem) om).setSelected( om.getText().equals( selected.toString() ) ) );
    }
  }

  private void resetTimeline()
  {
    if (timeline != null)
    {
      timeline.stop();
      timeline = null;
    }
  }

  private void timeFast(int millis)
  {
    resetTimeline();
    if (time == null)
      time = LocalDateTime.now();
    timeline = new Timeline( new KeyFrame( Duration.millis( millis ), ae ->
    {
      clockPane.repaint( time );
      time = time.plusSeconds( 1 );
    } ) );

    timeline.setCycleCount( Animation.INDEFINITE );
    timeline.play();
  }

  private ToolBar createToolbar()
  {
    ComboBox<String> animation = new ComboBox<>( animations );
    animation.setValue( animations.get( 0 ) );
    animation.setOnAction( new EventHandler<ActionEvent>()
    {
      @Override
      public void handle(ActionEvent event)
      {
        int index = animations.indexOf( animation.getValue() );
        switch (index)
        {
        case 0:
          resetTimeline();
          break;

        case 1:
          resetTimeline();
          timeline = new Timeline( new KeyFrame( Duration.seconds( 1 ), ae ->
          {
            time = LocalDateTime.now();
            clockPane.repaint( time );
          } ) );
          timeline.setCycleCount( Animation.INDEFINITE );
          timeline.play();
          break;

        case 2:
          timeFast( 500 );
          break;

        case 3:
          timeFast( 200 );
          break;

        case 4:
          timeFast( 100 );
          break;

        case 5:
          timeFast( 40 );
          break;

        case 6:
          timeFast( 10 );
          break;
        }
      }
    } );
    ToolBar toolbar = new ToolBar( new Label( "Animation:" ), animation );
    return toolbar;
  }

  private TreeItem<ClockData> setupTree(ClockData clockData)
  {
    TreeItem<ClockData> item = new TreeItem<ClockData>( clockData );

    for (ClockData children : clockData.getChildren())
      item.getChildren().add( setupTree( children ) );

    return item;
  }

  protected void initializeMenu(MenuBar menuBar)
  {
    Menu menuFile = new Menu( "File" );

    MenuItem openMenu = new MenuItem( "Open File" );
    openMenu.setOnAction( new EventHandler<ActionEvent>()
    {
      @Override
      public void handle(ActionEvent event)
      {
        fileOpen();
      }
    } );
    openMenu.setAccelerator( new KeyCodeCombination( KeyCode.O, KeyCodeCombination.CONTROL_DOWN ) );
    saveAsMenu = new MenuItem( "Save As" );
    saveAsMenu.setOnAction( new EventHandler<ActionEvent>()
    {
      @Override
      public void handle(ActionEvent event)
      {
        fileSaveAs();
      }
    } );

    saveMenu = new MenuItem( "Save" );
    saveMenu.setOnAction( new EventHandler<ActionEvent>()
    {
      @Override
      public void handle(ActionEvent event)
      {
        save( dataFile );
      }
    } );
    saveMenu.setDisable( true );
    saveMenu.setAccelerator( new KeyCodeCombination( KeyCode.S, KeyCodeCombination.CONTROL_DOWN ) );

    exportMenu = new MenuItem( "Export" );
    exportMenu.setOnAction( new EventHandler<ActionEvent>()
    {
      @Override
      public void handle(ActionEvent event)
      {
        export();
      }
    } );
    exportMenu.setDisable( true );
    menuFile.getItems().addAll( openMenu, saveMenu, saveAsMenu, exportMenu );

    menuBar.getMenus().addAll( menuFile );

    setTitle();
  }

  void setTitle()
  {
    mainStage.setTitle( "Designer " + (dataFile == null ? "" : dataFile.toString()) );

    saveMenu.setDisable( dataFile == null );
    exportMenu.setDisable( dataFile == null );
  }

  protected void fileOpen()
  {
    FileChooser fileChooser = new FileChooser();
    if (initialDirectory != null && initialDirectory.exists())
      fileChooser.setInitialDirectory( initialDirectory );
    fileChooser.setTitle( "Open" );
    fileChooser.getExtensionFilters().add( new ExtensionFilter( "Led Clock Designer file", "*.xml" ) );
    File file = fileChooser.showOpenDialog( mainStage );

    if (file != null)
    {
      try
      {
        JAXBContext jaxbContext = JAXBContext.newInstance( Clock.class );
        Unmarshaller unmarshaller = jaxbContext.createUnmarshaller();

        clock = (Clock) unmarshaller.unmarshal( file );

        dataFile = file;
        initialDirectory = dataFile.getParentFile();
        setTitle();

        clockPane.setClockPainter( null );
        clockPane.paint();

        root = setupTree( clock );
        tree.setRoot( root );
      }
      catch (JAXBException e)
      {
        e.printStackTrace();

        Alert alert = new Alert( AlertType.ERROR, "Error opening file " + e.getMessage(), ButtonType.OK );
        alert.showAndWait();
      }
    }
  }

  protected void fileSaveAs()
  {
    FileChooser fileChooser = new FileChooser();
    if (initialDirectory != null && initialDirectory.exists())
      fileChooser.setInitialDirectory( initialDirectory );
    fileChooser.setTitle( "Save As" );
    fileChooser.getExtensionFilters().add( new ExtensionFilter( "Save as file", "*.xml" ) );
    File file = fileChooser.showSaveDialog( mainStage );

    if (file != null)
      save( file );
  }

  protected void save(File file)
  {
    try
    {
      JAXBContext jaxbContext = JAXBContext.newInstance( Clock.class );
      Marshaller marshaller = jaxbContext.createMarshaller();

      marshaller.setProperty( Marshaller.JAXB_FORMATTED_OUTPUT, true );

      marshaller.marshal( clock, file );
      // marshaller.marshal( clock, System.out );

      dataFile = file;
      initialDirectory = dataFile.getParentFile();
      setTitle();
    }
    catch (JAXBException e)
    {
      e.printStackTrace();

      Alert alert = new Alert( AlertType.ERROR, "Error saving file " + e.getMessage(), ButtonType.OK );
      alert.showAndWait();
    }
  }

  protected void export()
  {
    FileChooser fileChooser = new FileChooser();
    if (initialDirectory != null && initialDirectory.exists())
      fileChooser.setInitialDirectory( initialDirectory );
    fileChooser.setTitle( "Export data" );
    fileChooser.getExtensionFilters().add( new ExtensionFilter( "Export", "*.cpp" ) );
    File file = fileChooser.showSaveDialog( mainStage );

    if (file != null)
      export( file );
  }

  private void export(File file)
  {
    final AtomicInteger index = new AtomicInteger( 0 );
    try (PrintWriter out = new PrintWriter( file ))
    {
      out.println();
      out.println( "#include \"Sprite.h\"" );
      out.println();

      clock.getClockFaces().getChildren().forEach( cf -> cf.export( out, index ) );

      out.println();
      out.println( "const SPRITE sprites[] PROGMEM = {" );
      out.print( "  " );
      
      index.set( 0 );
      clock.getClockFaces().getChildren().forEach( cf -> cf.exportStruct( out, index ) );

      out.println();
      out.println( "};" );

      out.println();
      out.print( "constexpr uint8_t SPRITES=" );
      out.print( 3 * clock.getClockFaces().getChildren().size() );
      out.println( ";" );

      out.println();
      out.println( "const uint8_t dials[] PROGMEM = {" );
      out.print( "  " );

      index.set( 0 );
      clock.getClockDials().getChildren().forEach( cd -> cd.export( out, index.getAndIncrement() ) );
      out.println( "};" );
      
      out.println();
      out.print( "constexpr uint8_t DIALS=" );
      out.print( clock.getClockDials().getChildren().size() );
      out.println( ";" );
    }
    catch (FileNotFoundException e)
    {
      e.printStackTrace();

      Alert alert = new Alert( AlertType.ERROR, "Error saving file " + e.getMessage(), ButtonType.OK );
      alert.showAndWait();
    }

  }

  public static void main(String[] args)
  {
    launch( args );
  }
}
