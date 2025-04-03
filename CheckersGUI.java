import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

public class CheckersGUI extends JFrame {
    private JPanel mainPanel;
    private JPanel welcomePanel;
    private JPanel boardPanel;
    private JPanel statsPanel;
    private CardLayout cardLayout;
    private CheckersGame game;

    private JLabel playerOneCaptureLabel;
    private JLabel playerTwoCaptureLabel;

    private int selectedRow = -1;
    private int selectedCol = -1;
    private int pieceRow = -1;
    private int pieceCol = -1;
    private boolean isPieceSelected = false;

    public CheckersGUI() {
        game = new CheckersGame();

        setTitle("Checkers Game");
        setSize(600, 650);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        cardLayout = new CardLayout();
        mainPanel = new JPanel(cardLayout);

        initializeWelcomeScreen();
        initializeGameBoard();
        initializeStatsPanel();

        JPanel gamePanel = new JPanel(new BorderLayout());
        gamePanel.add(boardPanel, BorderLayout.CENTER);
        gamePanel.add(statsPanel, BorderLayout.SOUTH);

        mainPanel.add(welcomePanel, "Welcome");
        mainPanel.add(gamePanel, "Game");

        setContentPane(mainPanel);
        cardLayout.show(mainPanel, "Welcome");

        setVisible(true);
    }

    private void initializeWelcomeScreen() {
        welcomePanel = new JPanel(new GridBagLayout());
        welcomePanel.setBackground(Color.LIGHT_GRAY);

        JLabel titleLabel = new JLabel("Welcome to Checkers", SwingConstants.CENTER);
        titleLabel.setFont(new Font("Serif", Font.BOLD, 28));

        JButton startButton = new JButton("Start Game");
        startButton.setFont(new Font("Arial", Font.PLAIN, 18));
        startButton.setPreferredSize(new Dimension(150, 50));

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.insets = new Insets(10, 0, 20, 0);
        gbc.anchor = GridBagConstraints.CENTER;

        welcomePanel.add(titleLabel, gbc);
        gbc.gridy = 1;
        gbc.insets = new Insets(0, 0, 0, 0);
        welcomePanel.add(startButton, gbc);

        startButton.addActionListener(e -> {
            cardLayout.show(mainPanel, "Game");
            boardPanel.requestFocusInWindow();
        });
    }

    private void initializeStatsPanel() {
        statsPanel = new JPanel(new GridLayout(1, 2));
        statsPanel.setPreferredSize(new Dimension(600, 50));

        playerOneCaptureLabel = new JLabel("Player One (White) Captured: 0", SwingConstants.CENTER);
        playerTwoCaptureLabel = new JLabel("Player Two (Black) Captured: 0", SwingConstants.CENTER);

        statsPanel.add(playerOneCaptureLabel);
        statsPanel.add(playerTwoCaptureLabel);
    }

    private void initializeGameBoard() {
        boardPanel = new JPanel(new GridLayout(8, 8));

        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                int finalRow = row;
                int finalCol = col;

                JPanel square = new JPanel() {
                    @Override
                    protected void paintComponent(Graphics g) {
                        super.paintComponent(g);
                        int[][] boardState = game.fetchBoardState();
                        int piece = boardState[finalRow][finalCol];

                        if (piece == 1) {
                            drawCircle(g, Color.WHITE); // white piece
                        } else if (piece == 3) {
                            drawCircle(g, Color.BLACK); // black piece
                        }

                        if (piece == 2) {
                            drawCircle(g, Color.WHITE);
                            drawKingCircle(g);
                        } else if (piece == 4) {
                            drawCircle(g, Color.BLACK);
                            drawKingCircle(g);
                        }

                        if (selectedRow == finalRow && selectedCol == finalCol) {
                            g.setColor(Color.YELLOW);
                            g.drawRect(1, 1, getWidth() - 2, getHeight() - 2);
                            g.drawRect(2, 2, getWidth() - 4, getHeight() - 4);
                        }

                        if (isPieceSelected && pieceRow == finalRow && pieceCol == finalCol) {
                            g.setColor(Color.GREEN);
                            g.drawRect(3, 3, getWidth() - 6, getHeight() - 6);
                            g.drawRect(4, 4, getWidth() - 8, getHeight() - 8);
                        }
                    }

                    private void drawCircle(Graphics g, Color color) {
                        g.setColor(color);
                        int padding = 10;
                        int diameter = Math.min(getWidth(), getHeight()) - padding;
                        g.fillOval(padding / 2, padding / 2, diameter, diameter);
                    }

                    private void drawKingCircle(Graphics g) {
                        g.setColor(Color.RED);
                        int padding = 10;
                        int mainDiameter = Math.min(getWidth(), getHeight()) - padding;
                        int kingDiameter = mainDiameter / 3;

                        int x = (getWidth() - kingDiameter) / 2;
                        int y = (getHeight() - kingDiameter) / 2;
                        g.fillOval(x, y, kingDiameter, kingDiameter);
                    }
                };

                if ((row + col) % 2 == 0) {
                    square.setBackground(Color.LIGHT_GRAY);
                } else {
                    square.setBackground(Color.DARK_GRAY);
                }

                square.addMouseListener(new MouseAdapter() {
                    @Override
                    public void mouseClicked(MouseEvent e) {
                        handleSquareClick(finalRow, finalCol);
                    }
                });

                boardPanel.add(square);
            }
        }

        boardPanel.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                switch (e.getKeyCode()) {
                    case KeyEvent.VK_UP:
                        selectedRow = Math.max(0, selectedRow - 1);
                        break;
                    case KeyEvent.VK_DOWN:
                        selectedRow = Math.min(7, selectedRow + 1);
                        break;
                    case KeyEvent.VK_LEFT:
                        selectedCol = Math.max(0, selectedCol - 1);
                        break;
                    case KeyEvent.VK_RIGHT:
                        selectedCol = Math.min(7, selectedCol + 1);
                        break;
                    case KeyEvent.VK_ENTER:
                    case KeyEvent.VK_SPACE:
                        handleSquareClick(selectedRow, selectedCol);
                        break;
                }
                refreshBoard();
            }
        });

        boardPanel.setFocusable(true);
        boardPanel.requestFocusInWindow();
    }

    private void handleSquareClick(int row, int col) {
        int[][] boardState = game.fetchBoardState();
        int piece = boardState[row][col];
        int currentPlayer = game.getCurrentPlayer();

        if (!isPieceSelected) {
            if ((currentPlayer == 1 && (piece == 1 || piece == 2)) || (currentPlayer == 3 && (piece == 3 || piece == 4))) {
                pieceRow = row;
                pieceCol = col;
                isPieceSelected = true;
                System.out.println("Selected piece at: (" + pieceRow + ", " + pieceCol + ")");
            } else {
                System.out.println("It's not your turn to move that piece!");
            }
        } else {
            if (row == pieceRow && col == pieceCol) {
                System.out.println("Deselected piece at: (" + pieceRow + ", " + pieceCol + ")");
                isPieceSelected = false;
                pieceRow = -1;
                pieceCol = -1;
            } else if ((currentPlayer == 1 && (piece == 1 || piece == 2)) || (currentPlayer == 3 && (piece == 3 || piece == 4))) {
                pieceRow = row;
                pieceCol = col;
                System.out.println("Reselected piece at: (" + pieceRow + ", " + pieceCol + ")");
            } else {
                boolean isValidMove = game.validateMove(pieceRow, pieceCol, row, col);
                if (isValidMove) {
                    game.executeMove(pieceRow, pieceCol, row, col);
                    isPieceSelected = false;
                    refreshBoard();

                    if (game.isGameOver()) {
                        handleGameOver();
                    }
                } else {
                    System.out.println("Invalid move from (" + pieceRow + ", " + pieceCol + ") to (" + row + ", " + col + ")");
                }
            }
        }

        selectedRow = row;
        selectedCol = col;
        refreshBoard();
    }

    private void refreshBoard() {
        boardPanel.repaint();
        int playerOneCaptured = game.getPlayerOneCaptured();
        int playerTwoCaptured = game.getPlayerTwoCaptured();

        playerOneCaptureLabel.setText("Player One (White) Captured: " + playerOneCaptured);
        playerTwoCaptureLabel.setText("Player Two (Black) Captured: " + playerTwoCaptured);
    }

    private void handleGameOver() {
        String winner = game.getWinner() == 1 ? "Player One (White)" : "Player Two (Black)";
        int result = JOptionPane.showOptionDialog(this, winner + " wins!", "Game Over",
                JOptionPane.DEFAULT_OPTION, JOptionPane.INFORMATION_MESSAGE,
                null, new String[]{"Restart", "Exit"}, "Restart");

        if (result == 0) {
            game = new CheckersGame();
            isPieceSelected = false;
            selectedRow = -1;
            selectedCol = -1;
            refreshBoard();
        } else {
            System.exit(0);
        }
    }
}