
package org.magnos.steer.behavior;

import java.awt.Color;
import java.awt.Graphics2D;

import org.magnos.steer.SteerSet;
import org.magnos.steer.obstacle.Plane;
import org.magnos.steer.spatial.SpatialDatabase;
import org.magnos.steer.spatial.SpatialEntityObstacle;
import org.magnos.steer.spatial.array.SpatialArray;
import org.magnos.steer.test.SteerSprite;
import org.magnos.steer.vec.Vec2;

import com.gameprogblog.engine.Game;
import com.gameprogblog.engine.GameLoop;
import com.gameprogblog.engine.GameLoopVariable;
import com.gameprogblog.engine.GameScreen;
import com.gameprogblog.engine.GameState;
import com.gameprogblog.engine.Scene;


public class SteerAvoidWallExample extends SteerBasicExample
{

	public static long GROUP_WALL = 1L << 0;
	
	public static void main( String[] args )
	{
		Game game = new SteerAvoidWallExample( DEFAULT_WIDTH, DEFAULT_HEIGHT );
		GameLoop loop = new GameLoopVariable( 0.1f );
		GameScreen screen = new GameScreen( DEFAULT_WIDTH, DEFAULT_HEIGHT, true, loop, game );
		screen.setBackground( Color.black );
		GameScreen.showWindow( screen, "SteerAvoidExample" );
	}

	private SpatialDatabase<Vec2> database;
	private SteerAvoidObstacles<Vec2> obstacles;
	private SteerSprite sprite;

	public SteerAvoidWallExample( int w, int h )
	{
		super( w, h );
		
		this.wrapEntities = false;
	}

	@Override
	public void start( Scene scene )
	{
        database = new SpatialArray<Vec2>( 16 );
	    
        Plane<Vec2> sideL = new Plane<Vec2>( new Vec2(0, DEFAULT_HEIGHT / 2), Vec2.RIGHT );
        Plane<Vec2> sideR = new Plane<Vec2>( new Vec2(DEFAULT_WIDTH, DEFAULT_HEIGHT / 2), Vec2.LEFT );
        Plane<Vec2> sideT = new Plane<Vec2>( new Vec2(DEFAULT_WIDTH / 2, 0), Vec2.TOP );
        Plane<Vec2> sideB = new Plane<Vec2>( new Vec2(DEFAULT_WIDTH / 2, DEFAULT_HEIGHT), Vec2.BOTTOM );
        
        database.add( new SpatialEntityObstacle<Vec2>( sideL, new Vec2(), GROUP_WALL, 0, true ) ); // left side
        database.add( new SpatialEntityObstacle<Vec2>( sideR, new Vec2(), GROUP_WALL, 0, true ) ); // right side
        database.add( new SpatialEntityObstacle<Vec2>( sideT, new Vec2(), GROUP_WALL, 0, true ) ); // top side
        database.add( new SpatialEntityObstacle<Vec2>( sideB, new Vec2(), GROUP_WALL, 0, true ) ); // bottom side
		
		sprite = newSprite( Color.blue, 15, 300, 1000, new SteerSet<Vec2>( 900,
			obstacles = new SteerAvoidObstacles<Vec2>( database, 80.0f, 30.0f, Vec2.FACTORY ),
			new SteerWander2( 0, 100, 150, 80 )
		));
		sprite.drawWrapped = false;
	}

	@Override
	public void draw( GameState state, Graphics2D gr, Scene scene )
	{
		super.draw( state, gr, scene );

		drawLine( gr, Color.blue, sprite.getPosition(), obstacles.lookaheadPoint, false );
	}

}
