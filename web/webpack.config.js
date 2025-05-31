const path = require('path');
const HtmlWebpackPlugin = require("html-webpack-plugin");
const MiniCssExtractPlugin = require('mini-css-extract-plugin');
const TerserPlugin = require('terser-webpack-plugin');
const CssMinimizerPlugin = require('css-minimizer-webpack-plugin');
const CopyWebpackPlugin = require('copy-webpack-plugin');
const CompressionPlugin = require("compression-webpack-plugin");
const Dotenv = require('dotenv-webpack');

module.exports = (env, argv) => {
    const isProduction = process.env.NODE_ENV === 'production';
    const isAcceptance = process.env.NODE_ENV === 'acceptance';
    const dotenvFilename = isProduction ? '.env.production' : (isAcceptance ? '.env.acceptance' :'.env.development');
    return {
        entry: './src/index.js',
        output: {
            filename: 'bundle.js',
            path: isProduction ? path.resolve(__dirname, '../data') : path.resolve(__dirname, 'dist'),
            clean: true,
        },

        mode: isProduction ? 'production' : 'development',
        devtool: isProduction ? false : 'inline-source-map',
        module: {
            rules: [
                {
                    test: /\.css$/,
                    use: [
                        isProduction ? MiniCssExtractPlugin.loader : 'style-loader',
                        'css-loader',
                    ],
                },
                {
                    test: /\.(js|jsx)$/,
                    exclude: /node_modules/,
                    loader: "babel-loader",
                    options: {
                      presets: [
                        "@babel/preset-env",
                        ["@babel/preset-react", { runtime: "automatic" }],
                      ],
                    },
                },
            ],
        },
        plugins: [
            new Dotenv({
                path: dotenvFilename,
              }),
            new MiniCssExtractPlugin({
                filename: 'styles.css',
            }),
            new CopyWebpackPlugin({
                patterns: [
                    { from: path.resolve(__dirname, '../config'), to: path.resolve(__dirname, '../data'),
                        noErrorOnMissing: true,
                         globOptions: {
                             deep: 1,
                             ignore: isProduction ? ['examples'] : ['*.*'],
                           },
                     },
                ],
            }),
            new TerserPlugin({
                terserOptions: {
                    compress: {
                        drop_console: false,
                    },
                },
            }),
            new CompressionPlugin({
               test: /\.js(\?.*)?$/i,
               deleteOriginalAssets: isProduction? true : false,
              }),
              new HtmlWebpackPlugin({
                filename: "index.html",
                template: "./public/index.html",
              }),
        ],
        optimization: {
            minimize: isProduction,
            minimizer: [
                new TerserPlugin(),
                new CssMinimizerPlugin(),
            ],
        },
        devServer: {
            static: {
                directory: path.join(__dirname, './public')
              },
              compress: true,
              historyApiFallback: true,
              open: true,
              hot: true,
              port: 9002,
        },
    };
};