public class CheckersGame {
    static {
        System.load("/Users/danylooliinyk/programming/Projects/Checkers/checkers_c++/cmake-build-debug/libCheckers_C__.dylib"); // the shared library
    }

    native long initGame();
    native int[][] getBoardState(long gamePtr);
    public native boolean validateMove(long gamePtr, int startX, int startY, int endX, int endY);
    public native void executeMove(long gamePtr, int startX, int startY, int endX, int endY);
    private native int getPlayerOneCaptured(long gamePtr);
    private native int getPlayerTwoCaptured(long gamePtr);
    native boolean isGameOver(long gamePtr);
    native int getWinner(long gamePtr);
    public native int getCurrentPlayer(long gamePtr);


    public boolean isGameOver() {
        return isGameOver(gamePtr);
    }

    public int getWinner() {
        return getWinner(gamePtr);
    }

    public int getCurrentPlayer() {
        return getCurrentPlayer(gamePtr);
    }

    private long gamePtr;

    public CheckersGame() {
        gamePtr = initGame();
    }

    public int[][] fetchBoardState() {
        return getBoardState(gamePtr);
    }

    public int getPlayerOneCaptured() {
        return getPlayerOneCaptured(gamePtr);
    }

    public int getPlayerTwoCaptured() {
        return getPlayerTwoCaptured(gamePtr);
    }

    public boolean validateMove(int startX, int startY, int endX, int endY) {
        return validateMove(gamePtr, startX, startY, endX, endY);
    }

    public void executeMove(int startX, int startY, int endX, int endY) {
        executeMove(gamePtr, startX, startY, endX, endY);
    }
}